#include "Thread.h"

using namespace sys;

#ifdef LINUX_PLATFORM

Thread::Thread(Thread &&val) noexcept
{
    m_thread = val.m_thread;
    val.m_thread = pthread_t();
    m_joianble = val.m_joianble;
    val.m_joianble = 0;

    m_can_destroy = val.m_can_destroy;
    val.m_can_destroy = 0;
}

const Thread &Thread::operator=(Thread &&val) noexcept
{
    m_thread = val.m_thread;
    val.m_thread = pthread_t();
    m_joianble = val.m_joianble;
    val.m_joianble = 0;

    m_can_destroy = val.m_can_destroy;
    val.m_can_destroy = 0;
    return *this;
}

Thread::~Thread()
{
    if (m_can_destroy)
    {
        pthread_cancel(m_thread);
    }
}

bool Thread::joinable() const
{
    return m_joianble;
}

bool Thread::join()
{
    if (pthread_join(m_thread, nullptr))
    {
        return 0;
    }
    m_joianble = 0;
    m_can_destroy = 0;
    return 1;
}

Thread::Thread(void *(*func)(void *), void *params)
{
    int err = pthread_create(
        &m_thread,
        nullptr,
        func,
        params);
    if (err)
    {
        std::cerr << "Failed to create thread\n";
    }
    m_can_destroy = 1;
    m_joianble = 1;
}
#endif

#ifdef WINDOWS_PLATFORM

Thread::Thread(Thread &&val) noexcept
{
    m_thread_handle = val.m_thread_handle;
    val.m_thread_handle = nullptr;

    m_joianble = val.m_joianble;
    val.m_joianble = 0;

    m_thread_id = val.m_thread_id;
    val.m_thread_id = 0;

    m_can_destroy = val.m_can_destroy;
    val.m_can_destroy = 0;
}

const Thread &Thread::operator=(Thread &&val) noexcept
{
    m_thread_handle = val.m_thread_handle;
    val.m_thread_handle = nullptr;

    m_joianble = val.m_joianble;
    val.m_joianble = 0;

    m_thread_id = val.m_thread_id;
    val.m_thread_id = 0;

    m_can_destroy = val.m_can_destroy;
    val.m_can_destroy = 0;
    return *this;
}

Thread::~Thread()
{
    if (m_can_destroy)
    {
        CloseHandle(m_thread_handle);
    }
}

bool Thread::joinable() const
{
    return m_joianble;
}

bool Thread::join()
{
    if (WaitForSingleObject(m_thread_handle, INFINITE) == WAIT_FAILED)
    {
        return 0;
    }
    m_joianble = 0;
    m_can_destroy = 0;
    return 1;
}

Thread::Thread(void *(*func)(void *), void *params)
{
    m_thread_handle = CreateThread(
        NULL,
        0,
        (PTHREAD_START_ROUTINE)func,
        params,
        0,
        &m_thread_id);
    if (m_thread_handle == nullptr)
    {
        std::cerr << "Failed to create thread\n";
    }
    m_can_destroy = 1;
    m_joianble = 1;
}
#endif
