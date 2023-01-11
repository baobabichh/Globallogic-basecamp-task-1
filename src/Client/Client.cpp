#include "Client.h"

#include "Functions/Functions.h"

namespace app
{
    Client::Client()
    {
    }

    Client::~Client()
    {
    }

    bool Client::connect(const std::string &ip, unsigned short port)
    {
        if (m_socket.connect(ip, port))
            return 1;
        return 0;
    }

    bool Client::wait(const std::string &ip, unsigned short port)
    {
        net::TcpListener listener;
        if (listener.listen(ip, port))
        {
            if (listener.accept(m_socket))
                return 1;
            else
                return 0;
        }
        return 0;
    }

    void Client::run()
    {
        char buffer[1024] = {0};

        while (m_should_run)
        {
            sys::SearchResult result;

            memset(buffer, 0, sizeof(buffer));
            if (!m_socket.receive(buffer, sizeof(buffer)))
            {
                LOG("CLIENT", "Disconnected!");
                m_socket.disconnect();
                m_should_run = 0;
                break;
            }

            m_searcher.search_for_directory(std::string(buffer), result);

            while (!result.ready())
            {
                if (!m_socket.send(REPLY_PROCCESING, sizeof(REPLY_PROCCESING)))
                {
                    LOG("CLIENT", "Disconnected!");
                    m_searcher.stop_search();
                    m_socket.disconnect();
                    m_should_run = 0;
                    break;
                }
                LOG("CLIENT", "Proccesing!");
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }

            std::string path = std::move(result.get());
            if (path == "")
            {
                path = REPLY_NO_PATH;
                LOG("CLIENT", "Directory not found!");
            }
            else
            {
                LOG("CLIENT", "Directory found : " << path);
            }

            if (!m_socket.send(&path[0], path.length() + 1))
            {
                LOG("CLIENT", "Disconnected!");
                m_socket.disconnect();
                m_should_run = 0;
                break;
            }
            
        }
    }
}