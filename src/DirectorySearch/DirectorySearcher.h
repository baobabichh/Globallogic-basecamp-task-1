#pragma once

#include <iostream>
#include <filesystem>
#include <string.h>
#include <array>
#include <thread>
#include <bitset>
#include <queue>
#include <atomic>
#include <mutex>
#include "PlatformDetection.h"
#include "Thread/Thread.h"

#define MAX_THREADS 4

namespace sys
{
    class SearchResult
    {
        friend class DirectorySearcher;

    public:
        SearchResult() = default;
        ~SearchResult() = default;

        bool ready() const { return m_ready; }
        const std::string &get() const { return m_path; }
        std::string get() { return m_path; }

    private:
        std::atomic<bool> m_ready = 0;
        std::string m_path = "";
    };

    class DirectorySearcher;

    struct SearchThreadFunctionParams
    {
    public:
        DirectorySearcher *m_this = nullptr;
        size_t id = 0;
    };

    class DirectorySearcher
    {
        friend void *search_thread_function(void *params);

    public:
        DirectorySearcher() = default;
        ~DirectorySearcher();

        void search_for_directory(const std::string &directory, SearchResult &result);
        void search_for_directory(const std::string &directory, const std::string &where, SearchResult &result);

        void stop_search();

        std::atomic_uint32_t count = 0;

    private:
        std::array<Thread, MAX_THREADS> m_threads;
        std::queue<std::string> m_directory_queue;
        std::atomic_uint32_t m_directory_queue_size = 0;
        std::mutex m_queue_mutex, m_result_mutex;
        std::array<std::atomic<bool>, MAX_THREADS> m_can_stop;
        std::atomic_bool m_should_work = 0;
        SearchResult *m_result = nullptr;
        std::string m_directory_to_search = "";
        std::array<SearchThreadFunctionParams, MAX_THREADS> m_threads_params;

    private:
        void start_search_thread(size_t id);
        void start_workers();
        void join_workers();
        void clear_queue();

        void process_dir(const std::string &path);
    };

    void *search_thread_function(void *params); // params contains 1. this , 2. thread index
}
