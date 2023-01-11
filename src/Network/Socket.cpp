#include "Socket.h"

#include <iostream>

namespace net
{

	Socket::Socket(Socket &&val) noexcept
		: m_id(val.m_id)
	{
		val.m_id = INVALID_SOCKET;
	}

	const Socket &Socket::operator=(Socket &&val) noexcept
	{
		m_id = val.m_id;
		val.m_id = INVALID_SOCKET;
		return *this;
	}

	Socket::~Socket()
	{
		destroy();
	}

	bool Socket::create()
	{
		m_id = socket(AF_INET, SOCK_STREAM, 0);
		if (m_id == INVALID_SOCKET)
		{
			return 0;
		}
		return 1;
	}

	void Socket::destroy()
	{
		if (m_id != INVALID_SOCKET)
			close_fd(m_id);
	}

}
