#include <iostream>
#include <filesystem>
#include "DirectorySearch/DirectorySearcher.h"
#include <string>
#include <vector>
#include <list>

#include "Network/TcpListener.h"
#include "Network/TcpSocket.h"
#include "Network/SocketSelector.h"
#include "Client/Client.h"
#include "Functions/Functions.h"
#include "Server/Server.h"
#include "Thread/Thread.h"

void run_client(bool ask)
{
    app::Client client;

    std::string ip = "127.0.0.1";
    unsigned short port = 54000;

    if (ask)
    {
        PRINT("Enter ip : ");
        ip = get_ip4();
        PRINT("Enter port : ");
        port = get_unsigned_short();
    }

    if (client.connect(ip, port))
    {
        LOG("CLIENT", "Connected!");
        client.run();
    }
    else
    {
        LOG("CLIENT", "Failed to connect!");
    }
}

void run_server(bool ask)
{

    std::string ip = "127.0.0.1";
    unsigned short port = 54000;

    if (ask)
    {
        PRINT("Enter ip : ");
        ip = get_ip4();
        PRINT("Enter port : ");
        port = get_unsigned_short();
    }

    app::Server server(ip, port);

    server.run();
}

void run()
{
    std::string str;
    std::cout << "Enter \'S\' for server , \'C\' for client : ";
    std::getline(std::cin, str);
    if (str == "S")
    {
        run_server(1);
    }
    else if (str == "C")
    {
        run_client(1);
    }
    else if (str == "SL")
    {
        run_server(0);
    }
    else if (str == "CL")
    {
        run_client(0);
    }
}

class MAKE
{
public:
    void make()
    {
        PRINT("MAKE " << val);
    }
    int val = 100;
};

struct A
{
    MAKE *m_this;
};

void *f1(void *params)
{
    A &val = *static_cast<A *>(params);
    val.m_this->make();

    return nullptr;
}

int main()
{

#ifdef WINDOWS_PLATFORM
    INIT_WINSOCK
#endif // WINDOWS_PLATFORM

    run();

    PRINT("Press Enter.");
    std::cin.get();

#ifdef WINDOWS_PLATFORM

    CLEANUP_WINSOCK
#endif // WINDOWS_PLATFORM
    return 0;
}