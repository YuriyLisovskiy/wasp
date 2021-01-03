/**
 * server/interfaces.h
 *
 * Copyright (c) 2020-2021 Yuriy Lisovskiy
 *
 * Purpose: TODO
 */

#pragma once

// Module definitions.
#include "./_def_.h"


__SERVER_BEGIN__

class IServer
{
public:
	// Bind socket.
	virtual void bind(const std::string& address, uint16_t port) = 0;

	// Listen the socket.
	virtual void listen(const std::string& startup_message) = 0;

	// Shutdown and close server socket.
	virtual void close() = 0;

	// Send all bytes.
	virtual core::Error send(int sock, const char* data) = 0;

	// Write n bytes from data array.
	virtual core::Error write(int sock, const char* data, size_t n) = 0;
};

__SERVER_END__
