#pragma once

#include "PlatformDetection.h"

#ifdef LINUX_PLATFORM

#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/select.h>
#include <unistd.h>

#define INVALID_SOCKET -1

#define close_fd ::close
typedef int socket_id;

#define SOCKET_ERROR -1

#endif // LINUX_PLATFORM

#ifdef WINDOWS_PLATFORM

#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#define INIT_WINSOCK                                                        \
	{                                                                       \
		WSADATA wsData;                                                     \
		WORD ver = MAKEWORD(2, 2);                                          \
		int wsOk = WSAStartup(ver, &wsData);                                \
		if (wsOk != 0)                                                      \
		{                                                                   \
			std::cerr << "Can't Initialize winsock! Quitting" << std::endl; \
			return -1;                                                      \
		}                                                                   \
	}

typedef SOCKET socket_id;

#define CLEANUP_WINSOCK \
	WSACleanup();

#define close_fd ::closesocket

#endif // WINDOWS_PLATFORM