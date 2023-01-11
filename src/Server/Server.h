#pragma once

#include <chrono>
#include "Network/SocketSelector.h"
#include <list>
#include <thread>
#include <atomic>
#include <iomanip>

#include "ServerClient.h"
#include "Functions/Functions.h"

#include "Thread/Thread.h"

static constexpr unsigned int MAX_REPLY_TIME = 1000; // in ms



namespace app
{
    class Server
    {
        friend void* selector_thread_function(void* params); // just selector_thread() method but this is function
        friend void* time_checker_thread_function(void* params); // just time_checker_thread() method but this is function
    public:
        Server(const std::string &ip, unsigned short port);
        ~Server();

        void run();
        void stop();

    private:
        unsigned int m_next_id = 0;
        std::atomic<bool> m_can_run = 0;
        net::TcpListener m_listener;
        net::SocketSelector m_selector;
        std::list<ServerClient> m_clients;
        std::mutex m_clients_mutex;
        sys::Thread m_selector_thread;
        sys::Thread m_time_checker_thread;

    private:
        void selector_thread();
        void time_checker_thread();
        bool send_request(ServerClient *client, const std::string &dir);
        void print_connections();
    };

    void* selector_thread_function(void* params); // just selector_thread() method but this is function
    void* time_checker_thread_function(void* params); // just time_checker_thread() method but this is function

}