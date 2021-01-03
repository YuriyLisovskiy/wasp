/**
 * core/server/util.cpp
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include "./util.h"

// C++ libraries.
#include <chrono>

// Core libraries.
#include <xalwart.core/exceptions.h>


__SERVER_UTIL_BEGIN__

std::string join_addr(const std::string& addr, uint16_t port)
{
	auto result = addr;
	if (port)
	{
		result += ":" + std::to_string(port);
	}

	return result;
}

std::string join_addr(const std::pair<std::string, uint16_t>& addr)
{
	return join_addr(addr.first, addr.second);
}

bool is_ipv6(const std::string& addr)
{
	return inet_pton(AF_INET6, addr.c_str(), nullptr) > 0;
}

std::shared_ptr<BaseSocket> create_socket(
	const std::string& address, uint16_t port, size_t retries_count, core::ILogger* logger
)
{
	std::function<std::shared_ptr<BaseSocket>()> make_socket;
	if (port)
	{
		if (is_ipv6(address))
		{
			make_socket = [address, port]() {
				return std::make_shared<TCP6Socket>(address.c_str(), port);
			};
		}
		else
		{
			make_socket = [address, port]() {
				return std::make_shared<TCPSocket>(address.c_str(), port);
			};
		}

	}
	else
	{
		make_socket = [address]() {
			return std::make_shared<UnixSocket>(address.c_str());
		};
	}

	std::shared_ptr<BaseSocket> socket;
	for (size_t i = 0; i < retries_count; i++)
	{
		try
		{
			socket = make_socket();
			break;
		}
		catch (const core::SocketError& exc)
		{
			switch (exc.err_no())
			{
				case EADDRINUSE:
					logger->error("Connection in use: " + join_addr(address, port));
					break;
				case EADDRNOTAVAIL:
					logger->error("Invalid address: " + join_addr(address, port));
					break;
			}

			if (i < retries_count)
			{
				logger->debug("connection to " + join_addr(address, port) + " failed: " + exc.what());
				logger->debug("Retrying in 1 second.");
				std::this_thread::sleep_for(std::chrono::seconds(1));
			}
		}
	}

	return socket;
}

void close_socket(
	std::shared_ptr<BaseSocket>& socket, core::ILogger* logger
)
{
	try
	{
		socket->close();
	}
	catch (const core::SocketError& exc)
	{
		logger->error("Error while closing socket: " + std::string(exc.what()));
	}
}

__SERVER_UTIL_END__
