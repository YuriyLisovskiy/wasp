/**
 * core/server/context.cpp
 *
 * Copyright (c) 2020-2021 Yuriy Lisovskiy
 */

#include "./context.h"


__SERVER_BEGIN__

void Context::normalize()
{
	if (!this->logger)
	{
		this->logger = core::Logger::get_instance({});
	}
}

__SERVER_END__
