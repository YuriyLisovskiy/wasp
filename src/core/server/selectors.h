/**
 * core/server/selectors.h
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 *
 * Purpose: TODO
 */

#pragma once

// C++ libraries.
#if defined(__linux__) || defined(__APPLE__)
//#include <arpa/inet.h>
#include <sys/socket.h>
//#include <sys/types.h>
//#include <unistd.h>
//#include <netdb.h>
#elif _WIN32
#include <winsock32.h>
#endif

// Core libraries.
#include <xalwart.core/logger.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
// TODO


__SERVER_BEGIN__

const int EVENT_READ = (1 << 0);
const int EVENT_WRITE = (1 << 1);

class ISelector
{
public:
	virtual void register_(int events) = 0;
	virtual bool select(int timeout) = 0;
};

class SelectSelector : public ISelector
{
protected:
	core::ILogger* logger;
	fd_set readers{};
	fd_set writers{};
	int fd;

public:
	explicit SelectSelector(int file_descriptor, core::ILogger* logger);
	void register_(int events) override;
	bool select(int timeout) override;
};

__SERVER_END__
