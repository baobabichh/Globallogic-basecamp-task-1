#pragma once

#include <mutex>
#include <chrono>
#include <Network/TcpSocket.h>
#include <memory>

#ifdef LINUX_PLATFORM

typedef std::chrono::_V2::system_clock::time_point TimePoint;

#endif // LINUX_PLATFORM

#ifdef WINDOWS_PLATFORM

typedef std::chrono::system_clock::time_point TimePoint;

#endif // WINDOWS_PLATFORM

namespace app
{

    static constexpr const char *STATE_TO_STRING[]{
        "CONNECTED",
        "SEARCHING",
    };

    enum class ServerClientState
    {
        CONNECTED,
        SEARCHING,
    };

    class ServerClient
    {
    public:
        ServerClient() = default;
        ServerClient(const ServerClient &val) = default;
        ServerClient(ServerClient &&val) = default;
        ~ServerClient() = default;
        void disconnect()
        {
            socket->disconnect();
        }

        ServerClientState state;
        TimePoint last_reply , first_reply;
        std::shared_ptr<net::TcpSocket> socket;
        unsigned int id = 0;
        std::string dir_request;
    };

}
