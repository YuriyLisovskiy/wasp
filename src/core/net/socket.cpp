///**
// * core/net/socket.cpp
// *
// * Copyright (c) 2019-2020 Yuriy Lisovskiy
// */
//
//#include "./socket.h"
//
//// C++ libraries.
//#include <fcntl.h>
//
//// Core libraries.
//#include <xalwart.core/exceptions.h>
//
//
//__NET_INTERNAL_BEGIN__
//
//Socket::Socket() : _socket(-1), _closed(true), _use_ipv6(false)
//{
//}
//
//socket_t Socket::create_ipv4(const char* host, uint16_t port)
//{
//	this->_ipv4_socket.sin_family = AF_INET;
//	this->_ipv4_socket.sin_port = htons(port);
//	::inet_pton(AF_INET, host, (void*) &this->_ipv4_socket.sin_addr.s_addr);
//	return ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//}
//
//socket_t Socket::create_ipv6(const char* host, uint16_t port)
//{
//	this->_ipv6_socket.sin6_family = AF_INET6;
//	this->_ipv6_socket.sin6_port = htons(port);
//	::inet_pton(PF_INET6, host, (void*) &this->_ipv6_socket.sin6_addr.s6_addr);
//	return ::socket(PF_INET6, SOCK_STREAM, IPPROTO_TCP);
//}
//
//int Socket::initialize() const
//{
//#if defined(_WIN32) || defined(_WIN64)
//	WSADATA wsaData;
//	return WSAStartup(MAKEWORD(2,2), &wsaData);
//#else
//	return 0;
//#endif
//}
//
//socket_t Socket::create(const char* host, uint16_t port, bool use_ipv6)
//{
//	this->_closed = false;
//	this->_use_ipv6 = use_ipv6;
//	if (this->_use_ipv6)
//	{
//		this->_socket = this->create_ipv6(host, port);
//	}
//	else
//	{
//		this->_socket = this->create_ipv4(host, port);
//	}
//
//	return this->_socket;
//}
//
//int Socket::bind()
//{
//	if (this->_use_ipv6)
//	{
//		return ::bind(this->_socket, (sockaddr*) &this->_ipv6_socket, sizeof(this->_ipv6_socket));
//	}
//
//	return ::bind(this->_socket, (sockaddr*) &this->_ipv4_socket, sizeof(this->_ipv4_socket));
//}
//
//int Socket::listen() const
//{
//	return ::listen(this->_socket, SOMAXCONN);
//}
//
//socket_t Socket::accept() const
//{
//	return ::accept(this->_socket, nullptr, nullptr);
//}
//
//int Socket::close() const
//{
//	if (this->_closed)
//	{
//		return 0;
//	}
//
//	return Socket::close_socket(this->_socket);
//}
//
//int Socket::set_reuse_addr() const
//{
//	const int true_flag = 1;
//#if defined(_WIN32) || defined(_WIN64)
//	return ::setsockopt(
//		this->_socket,
//		SOL_SOCKET,
//		SO_REUSEADDR,
//		(const char*) &true_flag,
//		sizeof(true_flag)
//	);
//#else
//	return ::setsockopt(
//		this->_socket,
//		SOL_SOCKET,
//		SO_REUSEADDR,
//		&true_flag,
//		sizeof(true_flag)
//	);
//#endif
//}
//
//int Socket::set_reuse_port() const
//{
//#if defined(_WIN32) || defined(_WIN64)
//	return 0;
//#else
//	const int true_flag = 1;
//	return ::setsockopt(
//		this->_socket,
//		SOL_SOCKET,
//		SO_REUSEPORT,
//		&true_flag,
//		sizeof(true_flag)
//	);
//#endif
//}
//
//bool Socket::set_blocking(bool blocking) const
//{
//	if (this->_closed)
//	{
//		throw SocketError("unable to set socket blocking, open socket first", _ERROR_DETAILS_);
//	}
//
//	if (this->_socket < 0)
//	{
//		return false;
//	}
//
//#if defined(_WIN32) || defined(_WIN64)
//	unsigned long mode = blocking ? 0 : 1;
//	return ioctlsocket(this->_socket, FIONBIO, &mode) == 0;
//#else
//	int flags = fcntl(this->_socket, F_GETFL, 0);
//	if (flags == -1)
//	{
//		return false;
//	}
//
//	flags = blocking ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK);
//	return fcntl(this->_socket, F_SETFL, flags) == 0;
//#endif
//}
//
//int Socket::close_socket(socket_t s)
//{
//	if (s != -1)
//	{
//#if defined(_WIN32) || defined(_WIN64)
//		return ::closesocket(s);
//#else
//		return ::close(s);
//#endif
//	}
//
//	return -1;
//}
//
//__NET_INTERNAL_END__
