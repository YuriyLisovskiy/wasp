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

// Framework libraries.
// TODO


__SERVER_BEGIN__

SelectSelector::SelectSelector(
	int file_descriptor, core::ILogger* logger
) : fd(file_descriptor), logger(logger)
{
	FD_ZERO(&this->readers);
	FD_ZERO(&this->writers);
}

void SelectSelector::register_(int events)
{
	if (events & EVENT_READ)
	{
		FD_ZERO(&this->readers);
		FD_SET(this->fd, &this->readers);
	}

	if (events & EVENT_WRITE)
	{
		FD_ZERO(&this->writers);
		FD_SET(this->fd, &this->writers);
	}
}

bool SelectSelector::select(int timeout)
{
	struct timeval t_val{};
	t_val.tv_sec = timeout;
	t_val.tv_usec = 0;
	int ret = ::select(this->fd + 1, &this->readers, &this->writers, nullptr, &t_val);
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
