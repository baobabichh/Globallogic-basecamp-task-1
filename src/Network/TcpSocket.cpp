#include "TcpSocket.h"

namespace net
{
    bool TcpSocket::connect(const std::string &ip, unsigned short port)
    {
        if (!create())
            return 0;

        memcpy(m_remote_ip, ip.c_str(), (std::min)(sizeof(m_remote_ip), ip.length() + 1));
        m_remote_port = port;

        sockaddr_in hint = {0};
        hint.sin_family = AF_INET;
        hint.sin_port = htons(m_remote_port);
        inet_pton(AF_INET, m_remote_ip, &hint.sin_addr);

        if (::connect(m_id, (sockaddr *)&hint, sizeof(hint)) == SOCKET_ERROR)
        {
            return 0;
        }
        return 1;
    }

    void TcpSocket::disconnect()
    {
        destroy();
    }

    bool TcpSocket::receive(void *buffer, size_t buffer_size)
    {
        int bytes = recv(m_id, (char *)buffer, buffer_size, 0);
        if (bytes <= 0)
        {
            return 0;
        }
        return 1;
    }

    bool TcpSocket::receive(void *buffer, size_t buffer_size, size_t &received)
    {
        int bytes = recv(m_id, (char *)buffer, buffer_size, 0);
        if (bytes <= 0)
        {
            return 0;
        }
        received = bytes;
        return 1;
    }

    bool TcpSocket::send(const void *buffer, size_t buffer_size)
    {
        int bytes = ::send(m_id, (char *)buffer, buffer_size, 0);
        if (bytes <= 0)
            return 0;
        return 1;
    }

    bool TcpSocket::send(void *buffer, size_t buffer_size, size_t &sent)
    {
        int bytes = ::send(m_id, (char *)buffer, buffer_size, 0);
        if (bytes <= 0)
            return 0;
        sent = bytes;
        return 1;
    }

    unsigned short TcpSocket::get_remote_port() const
    {
        return m_remote_port;
    }

    const char *TcpSocket::get_remote_address() const
    {
        return m_remote_ip;
    }

}
