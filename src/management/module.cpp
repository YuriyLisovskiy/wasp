/**
 * management/module.cpp
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 */

#include "./module.h"

// Framework libraries.
#include "./commands/start_server.h"


__MANAGEMENT_BEGIN__

CoreManagementModuleConfig::CoreManagementModuleConfig(
	conf::Settings* settings,
	std::function<std::shared_ptr<net::IServer>(
		core::ILogger*,
		collections::Dict<std::string, std::string>
	)> make_server
) : ModuleConfig(__FILE__, settings), _make_server(std::move(make_server))
{
}

void CoreManagementModuleConfig::commands()
{
	this->command<cmd::StartServerCommand>(this, this->settings, this->_make_server);
}

__MANAGEMENT_END__
