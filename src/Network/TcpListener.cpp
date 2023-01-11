#include "TcpListener.h"

namespace net
{
    bool TcpListener::accept(TcpSocket &socket)
    {
        sockaddr_in hint = {0};
        socklen_t len = sizeof(hint);

        auto val = ::accept(m_id, (struct sockaddr *)&hint, &len);
        if (val == SOCKET_ERROR)
        {
            return 0;
        }

        socket.m_id = val;
        inet_ntop(AF_INET, &(hint.sin_addr), &(socket.m_remote_ip[0]), INET_ADDRSTRLEN);
        socket.m_remote_port = ntohs(hint.sin_port);

        return 1;
    }

    bool TcpListener::listen(const std::string &ip, unsigned short port)
    {
        if (!create())
            return 0;

        sockaddr_in info;

        info.sin_family = AF_INET;
        info.sin_port = htons(port);
        inet_pton(AF_INET, ip.c_str(), &info.sin_addr);

        if (bind(m_id, (sockaddr *)&info, sizeof(info)) == SOCKET_ERROR)
            return 0;

        if (::listen(m_id, SOMAXCONN) == SOCKET_ERROR)
            return 0;
        return 1;
    }

    void TcpListener::close()
    {
        destroy();
    }

} // namespace net
