/**
 * commands/app_command.h
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 *
 * Purpose: the base class for AppConfig commands.
 */

#pragma once

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "./command.h"


__COMMANDS_BEGIN__

/// Derived commands must have a constructor
///	with pointer to apps::IAppConfig and pointer
/// to conf::Settings parameters.
class AppCommand : public Command
{
protected:
	apps::IAppConfig* app_config;

	AppCommand(
		apps::IAppConfig* app_cfg,
		conf::Settings* settings,
		const std::string& cmd_name,
		const std::string& help
	);
};

__COMMANDS_END__
