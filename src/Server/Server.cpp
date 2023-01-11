#include "Server/Server.h"

#include "Client/Client.h"

namespace app
{

    Server::Server(const std::string &ip, unsigned short port)
    {
        if (m_listener.listen(ip, port))
        {
            LOG("SERVER", "Listening ip : " << ip << " port : " << port);
            m_can_run = 1;
            m_selector.add_socket(m_listener);
        }
        else
        {
            LOG("SERVER", "Can't listen ip : " << ip << " port : " << port);
            m_can_run = 0;
        }
    }

    Server::~Server()
    {
    }

    void Server::selector_thread()
    {
        LOG("SERVER", "Selector thread started!");
        char buffer[1024] = {0};
        while (m_can_run)
        {
            if (m_selector.wait(1))
            {
                if (m_selector.is_ready(m_listener))
                {
                    // new connection
                    std::unique_ptr<net::TcpSocket> tmp_sock = std::make_unique<net::TcpSocket>();
                    if (m_listener.accept(*tmp_sock))
                    {
                        m_selector.add_socket(*tmp_sock);
                        ServerClient client;
                        client.id = m_next_id++;
                        client.state = ServerClientState::CONNECTED;
                        client.socket = std::move(tmp_sock);

                        m_clients_mutex.lock();
                        m_clients.push_back(client);
                        m_clients_mutex.unlock();

                        LOG("CLIENT CONNECTED",
                            "ID = " << client.id << " New connection!");
                    }
                }
                else
                {
                    m_clients_mutex.lock();
                    std::vector<std::list<ServerClient>::iterator> iterators_to_delete;
                    for (auto it = m_clients.begin(); it != m_clients.end(); it++)
                    {
                        auto &user = *it;
                        auto &sock = *(it->socket);
                        if (m_selector.is_ready(sock))
                        {
                            memset(buffer, 0, sizeof(buffer));
                            if (!sock.receive(buffer, sizeof(buffer)))
                            {
                                LOG("CLIENT DISCONNECTED",
                                    "ID = " << user.id << " Bad received data.");
                                user.disconnect();
                                m_selector.remove_socket(sock);
                                iterators_to_delete.push_back(it);
                                continue;
                            }
                            if (user.state == ServerClientState::SEARCHING)
                            {
                                if (strcmp(REPLY_PROCCESING, buffer) == 0)
                                {
                                    user.state = ServerClientState::SEARCHING;
                                    user.last_reply = std::chrono::system_clock::now();
                                    LOG("CLIENT PROCESSING",
                                        "ID = " << user.id << " Processing!");
                                }
                                else if (strcmp(REPLY_NO_PATH, buffer) == 0)
                                {
                                    user.state = ServerClientState::CONNECTED;
                                    auto now_time = std::chrono::system_clock::now();
                                    user.last_reply = now_time;
                                    unsigned int time = std::chrono::duration_cast<std::chrono::milliseconds>(now_time - user.first_reply).count();
                                    LOG("CLIENT FINISHED PROCESSING",
                                        "ID = " << user.id << " Finished processing, there is no such directory. Time : " << time << "ms.");
                                }
                                else
                                {
                                    user.state = ServerClientState::CONNECTED;
                                    auto now_time = std::chrono::system_clock::now();
                                    user.last_reply = now_time;
                                    unsigned int time = std::chrono::duration_cast<std::chrono::milliseconds>(now_time - user.first_reply).count();
                                    LOG("CLIENT FINISHED PROCESSING",
                                        "ID = " << user.id << " Directory found! Directory path : " << buffer << " Time : " << time << "ms.");
                                }
                            }
                        }
                    }
                    for (auto &it : iterators_to_delete)
                        m_clients.erase(it);
                    m_clients_mutex.unlock();
                }
            }
        }
        LOG("SERVER", "Selector thread stopped!");
    }

    void Server::time_checker_thread()
    {
        LOG("SERVER", "Time checker thread started!");
        while (m_can_run)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(MAX_REPLY_TIME));
            m_clients_mutex.lock(); // locking clients

            TimePoint now = std::chrono::system_clock::now();

            std::vector<std::list<ServerClient>::iterator> iterators_to_delete;
            for (auto it = m_clients.begin(); it != m_clients.end(); it++)
            {
                auto &user = *it;
                if (user.state == ServerClientState::SEARCHING)
                {
                    std::chrono::duration<double, std::milli> elapsed_ms = now - user.last_reply;

                    if (elapsed_ms.count() > MAX_REPLY_TIME)
                    {
                        LOG("CLIENT DISCONNECTED",
                            "ID = " << user.id << " Reply time elapsed.");
                        user.disconnect();
                        m_selector.remove_socket(*(user.socket));
                        iterators_to_delete.push_back(it);
                        continue;
                    }
                }
            }
            for (auto &it : iterators_to_delete)
                m_clients.erase(it);

            m_clients_mutex.unlock();
        }
        LOG("SERVER", "Time checker thread stopped!");
    }

    void Server::run()
    {
        if (m_can_run)
        {
            LOG("SERVER", "Server started!");
            m_selector_thread = std::move(sys::Thread(selector_thread_function, (void*)this));
            m_time_checker_thread = std::move(sys::Thread(time_checker_thread_function, (void*)this));
            while (m_can_run)
            {
                std::string tmp = "";
                std::getline(std::cin, tmp);
                auto cmd = get_command_type(tmp);
                if (cmd == Command::NONE)
                {
                    LOG("SERVER INPUT", "Wrong input!");
                }
                else
                {
                    if (cmd == Command::SEND)
                    {
                        unsigned int id;
                        std::string path;
                        if (parse_send_command(tmp, id, path))
                        {
                            m_clients_mutex.lock();
                            auto client = std::find_if(std::begin(m_clients), std::end(m_clients), [&](ServerClient &val)
                                                       { return val.id == id; });
                            if (client == std::end(m_clients))
                            {
                                LOG("SERVER INPUT", "Wrong send command! No client with ID = " << id);
                            }
                            else
                            {
                                if (this->send_request(&(*client), path))
                                {
                                    LOG("SERVER INPUT", "Success send ID = |" << id << "| DIR = |" << path << "|");
                                }
                                else
                                {
                                    LOG("SERVER INPUT", "Failed send ID = |" << id << "| DIR = |" << path << "|");
                                }
                            }
                            m_clients_mutex.unlock();
                        }
                        else
                        {
                            LOG("SERVER INPUT", "Wrong send command!");
                        }
                    }
                    else if (cmd == Command::SEND_ALL)
                    {
                        std::string path;
                        if (parse_send_all_command(tmp, path))
                        {
                            m_clients_mutex.lock();
                            for (auto &it : m_clients)
                            {
                                if (this->send_request(&it, path))
                                {
                                    LOG("SERVER INPUT", "Success send ID = |" << it.id << "| DIR = |" << path << "|");
                                }
                                else
                                {
                                    LOG("SERVER INPUT", "Failed send ID = |" << it.id << "| DIR = |" << path << "|");
                                }
                            }
                            if (m_clients.empty())
                            {
                                LOG("SERVER INPUT", "There are no connections.");
                            }
                            m_clients_mutex.unlock();
                        }
                        else
                        {
                            LOG("SERVER INPUT", "Wrong sendall command!");
                        }
                    }
                    else if (cmd == Command::STOP_SERVER)
                    {
                        LOG("SERVER INPUT", "Stop command!");
                        stop();
                    }
                    else if (cmd == Command::PRINT_CONNECTIONS)
                    {
                        LOG("SERVER INPUT", "Print connections info!");
                        print_connections();
                    }
                    else if (cmd == Command::DISCONNECT)
                    {
                        unsigned int id;
                        if (parse_disconnect_command(tmp, id))
                        {
                            m_clients_mutex.lock();
                            auto client = std::find_if(std::begin(m_clients), std::end(m_clients), [&](ServerClient &val)
                                                       { return val.id == id; });
                            if (client == std::end(m_clients))
                            {
                                LOG("SERVER INPUT", "Wrong disconnect command! No client with ID = " << id);
                            }
                            else
                            {
                                client->disconnect();
                                LOG("SERVER INPUT", "Success disconnect ID = |" << id << "|");
                                m_clients.erase(client);
                            }
                            m_clients_mutex.unlock();
                        }
                        else
                        {
                            LOG("SERVER INPUT", "Wrong disconnect command!");
                        }
                    }
                }
            }
        }
        LOG("SERVER", "Server stopped!");
    }

    void Server::stop()
    {
        m_clients_mutex.lock();
        m_clients.clear();
        m_selector.clear();
        m_clients_mutex.unlock();

        m_can_run = 0;

        if (m_selector_thread.joinable())
            m_selector_thread.join();
        if (m_time_checker_thread.joinable())
            m_time_checker_thread.join();
    }

    bool Server::send_request(ServerClient *client, const std::string &dir)
    {
        if (client && client->socket && client->state == ServerClientState::CONNECTED)
        {
            if (client->socket->send(&dir[0], dir.length() + 1))
            {
                auto time_now = std::chrono::system_clock::now();
                client->state = ServerClientState::SEARCHING;
                client->dir_request = dir;
                client->last_reply = time_now;
                client->first_reply = time_now;
                return 1;
            }
            else
            {
                return 0;
            }
        }
        return 0;
    }

    void Server::print_connections()
    {
        m_clients_mutex.lock();
        s_output_mutex.lock();
        for (auto &it : m_clients)
        {
            std::cout << "[SERVER INFO] "
                      << std::setw(8) << it.id
                      << std::setw(16) << STATE_TO_STRING[(int)it.state]
                      << std::setw(24) << it.socket->get_remote_address()
                      << std::setw(8) << it.socket->get_remote_port()
                      << "\n";
        }
        if (m_clients.empty())
        {
            std::cout << "[SERVER INFO] There are no connections.\n";
        }

        s_output_mutex.unlock();
        m_clients_mutex.unlock();
    }

    void* selector_thread_function(void* params)
    {
        Server* m_this = (Server*)params;

        m_this->selector_thread();

        return nullptr;
    }

    void* time_checker_thread_function(void* params)
    {
        Server* m_this = (Server*)params;

        m_this->time_checker_thread();

        return nullptr;
    }

} // namespace app
