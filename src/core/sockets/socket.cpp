/*
 * Copyright (c) 2019 Yuriy Lisovskiy
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
 * An implementation of socket.h.
 */

#include "socket.h"


__INTERNAL_BEGIN__

Socket::Socket(socket_t sock) : _socket(sock), _closed(true)
{
}

int Socket::close()
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

bool Socket::set_blocking(bool blocking)
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
	return ioctlsocket(fd, FIONBIO, &mode) == 0;
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

__INTERNAL_END__
