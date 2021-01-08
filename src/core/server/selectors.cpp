/**
 * core/server/selectors.cpp
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include "./selectors.h"

// C++ libraries.
#include <cerrno>
#include <string>
#include <cstring>


__SERVER_BEGIN__

SelectSelector::SelectSelector(core::ILogger* logger) : logger(logger)
{
	this->fd = -1;
	this->events = -1;
}

void SelectSelector::register_(uint fd, int events)
{
	this->fd = fd;
	this->events = events;

	if (this->events & EVENT_READ)
	{
		FD_ZERO(&this->readers);
		FD_SET(this->fd, &this->readers);
	}

	if (this->events & EVENT_WRITE)
	{
		FD_ZERO(&this->writers);
		FD_SET(this->fd, &this->writers);
	}
}

bool SelectSelector::select(uint timeout)
{
	auto fd_writers = this->writers;
	auto fd_readers = this->readers;
	struct timeval t_val{};
	t_val.tv_sec = timeout;
	t_val.tv_usec = 0;
	int ret = ::select(this->fd + 1, &fd_readers, &fd_writers, nullptr, &t_val);
	if (ret == -1)
	{
		this->logger->error(
			"'select' call failed: " + std::string(strerror(errno)), _ERROR_DETAILS_
		);
	}
	else if (ret == 0)
	{
		// timeout, just skip
	}
	else
	{
		return true;
	}

	return false;
}

__SERVER_END__
