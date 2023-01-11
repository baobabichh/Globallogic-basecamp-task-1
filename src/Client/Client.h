#pragma once

#include "Network/SocketSelector.h"
#include "DirectorySearch/DirectorySearcher.h"

static constexpr const char REPLY_PROCCESING[] = "processing"; 
static constexpr const char REPLY_NO_PATH[] = "NO PATH";

namespace app
{
    class Client
    {
    public:
        Client();
        ~Client();

        bool connect(const std::string &ip , unsigned short port);
        bool wait(const std::string &ip, unsigned short port);
        void run();

    private:
        net::TcpSocket m_socket;
        sys::DirectorySearcher m_searcher;
        bool m_should_run = 1;
    };
}
