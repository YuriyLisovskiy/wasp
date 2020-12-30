/**
 * management/app.cpp
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 */

#include "./app.h"

// Framework libraries.
#include "./commands/start_server.h"


__MANAGEMENT_BEGIN__

CoreManagementAppConfig::CoreManagementAppConfig(conf::Settings* settings)
	: AppConfig(__FILE__, settings)
{
	this->init(this->__type__());
}

void CoreManagementAppConfig::commands()
{
	this->command<cmd::StartServerCommand>();
}

__MANAGEMENT_END__
