#pragma once

#include <iostream>
#include <string.h>

#include "NetworkHeader.h"
#include "Socket.h"

namespace net
{
    class TcpSocket : public Socket
    {
        friend class SocketSelector;
        friend class TcpListener;

    public:
        TcpSocket() = default;
        ~TcpSocket() = default;

        bool connect(const std::string &ip, unsigned short port);
        void disconnect();

        bool receive(void *buffer, size_t buffer_size);
        bool receive(void *buffer, size_t buffer_size, size_t &received);
        bool send(const void *buffer, size_t buffer_size);
        bool send(void *buffer, size_t buffer_size, size_t &sent);

        unsigned short get_remote_port() const;
        const char *get_remote_address() const;

    private:
        char m_remote_ip[INET_ADDRSTRLEN + 1] = {0};
        unsigned short m_remote_port = {0};
    };
}
