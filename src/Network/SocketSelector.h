#pragma once

#include "TcpSocket.h"
#include "TcpListener.h"

namespace net
{
    class SocketSelector
    {
    public:
        SocketSelector();
        ~SocketSelector();

        void add_socket(Socket &socket);
        void remove_socket(Socket &socket);
        bool is_ready(Socket &socket);
        void clear();
        bool wait();
        bool wait(unsigned int time);

    private:
        fd_set m_fd_set, m_fd_set_copy;
    };
} // namespace net
