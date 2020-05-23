/*
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * An implementation of core/net/socket.h
 */

#include "./socket.h"

 // C++ libraries.
#include <fcntl.h>

// Framework modules.
#include "../exceptions.h"


__NET_INTERNAL_BEGIN__

/*
#if defined(_WIN32) || defined(_WIN64)
int inet_pton(int af, const char* src, void* dst)
{
	struct sockaddr_storage ss{};
	int size = sizeof(ss);
	char src_copy[INET6_ADDRSTRLEN + 1];

	ZeroMemory(&ss, sizeof(ss));
	strncpy(src_copy, src, INET6_ADDRSTRLEN + 1);
	src_copy[INET6_ADDRSTRLEN] = 0;

	if (WSAStringToAddress(src_copy, af, nullptr, (struct sockaddr*)&ss, &size) == 0)
	{
		switch (af)
		{
			case AF_INET:
				*(struct in_addr *) dst = ((struct sockaddr_in *) &ss)->sin_addr;
				return 1;
			case AF_INET6:
				*(struct in6_addr *) dst = ((struct sockaddr_in6 *) &ss)->sin6_addr;
				return 1;
		}
	}

	return 0;
}
#endif // _WIN32 || _WIN64
*/

Socket::Socket() : _socket(-1), _closed(true), _use_ipv6(false)
{
}

socket_t Socket::create_ipv4(const char* host, uint16_t port)
{
	this->_ipv4_socket.sin_family = AF_INET;
	this->_ipv4_socket.sin_port = ::htons(port);
	//this->_ipv4_socket.sin_addr.s_addr = ::inet_addr(host);
	::inet_pton(AF_INET, host, (void*) &this->_ipv4_socket.sin_addr.s_addr);
	return ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

socket_t Socket::create_ipv6(const char* host, uint16_t port)
{
	this->_ipv6_socket.sin6_family = AF_INET6;
	this->_ipv6_socket.sin6_port = ::htons(port);
	::inet_pton(PF_INET6, host, (void*) &this->_ipv6_socket.sin6_addr.s6_addr);
	return ::socket(PF_INET6, SOCK_STREAM, IPPROTO_TCP);
}

int Socket::initialize() const
{
#if defined(_WIN32) || defined(_WIN64)
	WSADATA wsaData;
	return WSAStartup(MAKEWORD(2,2), &wsaData);
#else
	return 0;
#endif
}

socket_t Socket::create(const char* host, uint16_t port, bool use_ipv6)
{
	this->_closed = false;
	this->_use_ipv6 = use_ipv6;
	if (this->_use_ipv6)
	{
		this->_socket = this->create_ipv6(host, port);
	}
	else
	{
		this->_socket = this->create_ipv4(host, port);
	}

	return this->_socket;
}

int Socket::bind()
{
	if (this->_use_ipv6)
	{
		return ::bind(this->_socket, (sockaddr*) &this->_ipv6_socket, sizeof(this->_ipv6_socket));
	}

	return ::bind(this->_socket, (sockaddr*) &this->_ipv4_socket, sizeof(this->_ipv4_socket));
}

int Socket::listen() const
{
	return ::listen(this->_socket, SOMAXCONN);
}

socket_t Socket::accept() const
{
	return ::accept(this->_socket, nullptr, nullptr);
}

int Socket::close() const
{
	if (this->_closed)
	{
		return 0;
	}

	if (this->_socket != -1)
	{
#if defined(_WIN32) || defined(_WIN64)
		return ::closesocket(this->_socket);
#elif defined(__unix__) || defined(__linux__)
		return ::close(this->_socket);
#else
#error Library is not supported on this platform
#endif
	}
	return -1;
}

int Socket::set_reuse_addr() const
{
	const int true_flag = 1;
#if defined(_WIN32) || defined(_WIN64)
	return ::setsockopt(
		this->_socket,
		SOL_SOCKET,
		SO_REUSEADDR,
		(const char*) &true_flag,
		sizeof(true_flag)
	);
#else
	return ::setsockopt(
		this->_socket,
		SOL_SOCKET,
		SO_REUSEADDR,
		&true_flag,
		sizeof(true_flag)
	);
#endif
}

int Socket::set_reuse_port() const
{
#if defined(_WIN32) || defined(_WIN64)
	return 0;
#else
	const int true_flag = 1;
	return ::setsockopt(
		this->_socket,
		SOL_SOCKET,
		SO_REUSEPORT,
		&true_flag,
		sizeof(true_flag)
	);
#endif
}

bool Socket::set_blocking(bool blocking) const
{
	if (this->_closed)
	{
		throw SocketError("unable to set socket blocking, open socket first", _ERROR_DETAILS_);
	}

	if (this->_socket < 0)
	{
		return false;
	}

#if defined(_WIN32) || defined(_WIN64)
	unsigned long mode = blocking ? 0 : 1;
	return ioctlsocket(this->_socket, FIONBIO, &mode) == 0;
#else
	int flags = fcntl(this->_socket, F_GETFL, 0);
	if (flags == -1)
	{
		return false;
	}

	flags = blocking ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK);
	return fcntl(this->_socket, F_SETFL, flags) == 0;
#endif
}

int Socket::close_socket(socket_t s)
{
	if (s != -1)
	{
		return ::closesocket(s);
	}

	return 0;
}

__NET_INTERNAL_END__
