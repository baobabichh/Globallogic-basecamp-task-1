#include "DirectorySearcher.h"
#include "PlatformDetection.h"
#include <string.h>
#include <algorithm>

#ifdef LINUX_PLATFORM

#include <dirent.h>

#endif // LINUX_PLATFORM

#ifdef WINDOWS_PLATFORM

#include <windows.h>

#endif // WINDOWS_PLATFORM

namespace sys
{
    void DirectorySearcher::search_for_directory(const std::string &directory, SearchResult &result)
    {
        search_for_directory(directory, ROOT_PATH, result);
    }

    void DirectorySearcher::search_for_directory(const std::string &directory, const std::string &where, SearchResult &result)
    {
        clear_queue();
        m_should_work = 1;
        result.m_ready = 0;
        result.m_path = std::string();
        m_directory_queue_size = 1;
        m_result = &result;
        m_directory_to_search = directory;
        m_directory_queue.push(where);

        std::fill(std::begin(m_can_stop), std::end(m_can_stop), 0);

        start_workers();
    }

    void DirectorySearcher::stop_search()
    {
        m_should_work = 0;
        join_workers();
        clear_queue();
    }

    DirectorySearcher::~DirectorySearcher()
    {
        join_workers();
    }

    void DirectorySearcher::start_workers()
    {
        m_should_work = 1;

        for (int i = 0; i < m_can_stop.size(); i++)
            m_can_stop[i] = 0;

        for (int i = 0; i < m_threads.size(); i++)
        {
            m_threads_params[i].m_this = this;
            m_threads_params[i].id = i;

            if (m_threads[i].joinable())
            {
                m_threads[i].join();
            }
            m_threads[i] = std::move(Thread(search_thread_function, (void*)&(m_threads_params[i])));
        }
    }

    void DirectorySearcher::join_workers()
    {
        m_should_work = 0;

        for (auto &it : m_threads)
        {
            if (it.joinable())
                it.join();
        }
    }

    void DirectorySearcher::clear_queue()
    {
        m_queue_mutex.lock();
        while (m_directory_queue.size())
        {
            m_directory_queue.pop();
        }
        m_queue_mutex.unlock();
    }

    void DirectorySearcher::start_search_thread(size_t id)
    {
        m_can_stop[id] = 0;

        std::string path;

        while (m_should_work)
        {
            m_queue_mutex.lock();
            if (!m_directory_queue.empty())
            {
                m_can_stop[id] = 0;
                path = std::move(m_directory_queue.front());
                m_directory_queue.pop();
                m_directory_queue_size--;
            }
            else
            {
                m_can_stop[id] = 1;
            }
            m_queue_mutex.unlock();

            if (!m_can_stop[id])
            {
                count++;
                process_dir(path);
            }
            else
            {
                if (!m_directory_queue_size)
                {
                    auto val = (size_t)std::count(std::begin(m_can_stop), std::end(m_can_stop), 1);
                    if (val == m_can_stop.size())
                    {
                        m_should_work = 0;
                        m_result_mutex.lock();
                        if (m_result)
                        {
                            m_result->m_ready = 1;
                            m_result->m_path = std::string();
                            m_result = nullptr;
                        }
                        m_result_mutex.unlock();
                    }
                }
            }
        }
    }

#ifdef LINUX_PLATFORM

    void DirectorySearcher::process_dir(const std::string &path)
    {
        DIR *dr = opendir(path.c_str());

        if (!dr)
            return;

        struct dirent *de;

        while ((de = readdir(dr)) != NULL)
        {
            if (de->d_type == DT_DIR)
            {
                const char *dir_name = de->d_name;
                std::string dir_path = path + '/' + dir_name;

                if (strcmp(dir_name, ".") == 0 || strcmp(dir_name, "..") == 0)
                    continue;

                if (strcmp(dir_name, m_directory_to_search.c_str()) == 0)
                {
                    m_should_work = 0;
                    m_result_mutex.lock();
                    if (m_result)
                    {
                        m_result->m_ready = 1;
                        m_result->m_path = dir_path;
                        m_result = nullptr;
                    }
                    m_result_mutex.unlock();
                    break;
                }
                else
                {
                    m_queue_mutex.lock();
                    m_directory_queue.push(dir_path);
                    m_directory_queue_size++;
                    m_queue_mutex.unlock();
                }
            }
        }

        closedir(dr);
    }

#endif // LINUX_PLATFORM

#ifdef WINDOWS_PLATFORM

    void DirectorySearcher::process_dir(const std::string &path)
    {
        std::string start_dir = path + "/*";

        HANDLE fileHandle;
        WIN32_FIND_DATAA ffd;
        fileHandle = FindFirstFileA(start_dir.c_str(), &ffd);

        if (INVALID_HANDLE_VALUE == fileHandle)
            return;

        do
        {
            if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                const char *dir_name = ffd.cFileName;
                std::string dir_path = path + "/" + dir_name;

                if (strcmp(dir_name, ".") == 0 || strcmp(dir_name, "..") == 0)
                    continue;

                if (strcmp(dir_name, m_directory_to_search.c_str()) == 0)
                {
                    m_should_work = 0;
                    m_result_mutex.lock();
                    if (m_result)
                    {
                        m_result->m_ready = 1;
                        m_result->m_path = dir_path;
                        m_result = nullptr;
                    }
                    m_result_mutex.unlock();
                    break;
                }
                else
                {
                    m_queue_mutex.lock();
                    m_directory_queue.push(dir_path);
                    m_directory_queue_size++;
                    m_queue_mutex.unlock();
                }
            }
        } while (FindNextFileA(fileHandle, &ffd) != 0);
    }

#endif // WINDOWS_PLATFORM

    void *search_thread_function(void *params)
    {
        SearchThreadFunctionParams &val = *static_cast<SearchThreadFunctionParams *>(params);
        val.m_this->start_search_thread(val.id);

        return nullptr;
    }

}
