
#include "SocketSelector.h"

namespace net
{
    SocketSelector::SocketSelector()
    {
        FD_ZERO(&m_fd_set);
        FD_ZERO(&m_fd_set_copy);
    }

    SocketSelector::~SocketSelector()
    {
        FD_ZERO(&m_fd_set);
        FD_ZERO(&m_fd_set_copy);
    }

    void SocketSelector::add_socket(Socket &socket)
    {
        FD_SET(socket.m_id, &m_fd_set);
    }

    void SocketSelector::remove_socket(Socket &socket)
    {
        FD_CLR(socket.m_id, &m_fd_set);
        FD_CLR(socket.m_id, &m_fd_set_copy);
    }

    void SocketSelector::clear()
    {
        FD_ZERO(&m_fd_set);
    }

    bool SocketSelector::is_ready(Socket &socket)
    {
        if (FD_ISSET(socket.m_id, &m_fd_set_copy))
            return 1;
        return 0;
    }

    bool SocketSelector::wait()
    {
        memcpy(&m_fd_set_copy, &m_fd_set, sizeof(fd_set));
        int socket_count = select(FD_SETSIZE, &m_fd_set_copy, nullptr, nullptr, nullptr);
        if (socket_count)
            return 1;
        return 0;
    }

    bool SocketSelector::wait(unsigned int seconds)
    {
        timeval timeout = {0};
        timeout.tv_sec = seconds;
        timeout.tv_usec = 0;
        memcpy(&m_fd_set_copy, &m_fd_set, sizeof(fd_set));
        int socket_count = select(FD_SETSIZE, &m_fd_set_copy, nullptr, nullptr, &timeout);
        if (socket_count)
            return 1;
        return 0;
    }
}