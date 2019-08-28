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

/*
 * server_socket definition.
 * TODO: write docs.
 */

#ifndef WASP_CORE_TCP_SERVER_SOCKET_H
#define WASP_CORE_TCP_SERVER_SOCKET_H

#include "../../globals.h"
#include "socket.h"
#include "../exceptions.h"

__INTERNAL_BEGIN__

class ServerSocket : public Socket
{
private:
	sockaddr_in _socketAddr{};

public:
	ServerSocket();

	socket_t create(const char* host, uint16_t port);
	int bind();
	int listen();
	socket_t accept();
};

__INTERNAL_END__


#endif // WASP_CORE_TCP_SERVER_SOCKET_H
