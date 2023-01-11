#pragma once

#include "TcpSocket.h"

namespace net
{
    class TcpListener : public Socket
    {
        friend class SocketSelector;

    public:
        TcpListener() = default;
        ~TcpListener() = default;

        bool accept(TcpSocket &socket);
        bool listen(const std::string &ip, unsigned short port);
        void close();
    };
}
