#pragma once

#include "PlatformDetection.h"
#include <iostream>

#ifdef LINUX_PLATFORM
#include <pthread.h>
#endif

#ifdef WINDOWS_PLATFORM

#define _WINSOCKAPI_ 
#include <windows.h>

#endif

namespace sys
{
    class Thread
    {
    public:
        Thread(void *(*func)(void *), void *params);
        Thread() = default;
        Thread(const Thread &val) = delete;
        Thread(Thread &&val) noexcept;
        const Thread &operator=(const Thread &val) = delete;
        const Thread &operator=(Thread &&val) noexcept;
        ~Thread();

        bool joinable() const;
        bool join();

    private:
#ifdef LINUX_PLATFORM
        pthread_t m_thread;
        bool m_joianble = 0;
        bool m_can_destroy = 0;
#endif

#ifdef WINDOWS_PLATFORM
        HANDLE m_thread_handle = nullptr;
        DWORD m_thread_id = 0;
        bool m_joianble = 0;
        bool m_can_destroy = 0;
#endif
    };

} // namespace sys
