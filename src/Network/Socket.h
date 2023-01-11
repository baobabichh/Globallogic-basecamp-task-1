#pragma once

#include "NetworkHeader.h"

namespace net
{
	class Socket
	{
		friend class SocketSelector;
	public:
		Socket() = default;
		Socket(const Socket &val) = delete;
		Socket(Socket &&val) noexcept;
		const Socket &operator=(const Socket &val) = delete;
		const Socket &operator=(Socket &&val) noexcept;
		virtual ~Socket();

	protected:
		socket_id m_id = INVALID_SOCKET;

	protected:
		bool create();
		void destroy();
	};
}