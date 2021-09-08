/**
 * management/module.cpp
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 */

#include "./module.h"

// Framework libraries.
#include "./commands/start_server.h"
#include "./commands/migrate.h"


__MANAGEMENT_BEGIN__

void CoreModuleConfig::commands()
{
	this->command<cmd::MigrateCommand>(this, this->settings);
	this->command<cmd::StartServerCommand>(this, this->settings, this->_handler_function);
}

__MANAGEMENT_END__
