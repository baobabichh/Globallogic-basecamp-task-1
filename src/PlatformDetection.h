#pragma once


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    #define WINDOWS_PLATFORM
#elif __linux__
    #define LINUX_PLATFORM
#else
#   error "Unknown compiler"
#endif

#ifdef LINUX_PLATFORM
    #define ROOT_PATH "/"
#endif

#ifdef WINDOWS_PLATFORM
    #define ROOT_PATH "C:/"
#endif

