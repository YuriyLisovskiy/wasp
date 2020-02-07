/*
 * Copyright (c) 2019 Yuriy Lisovskiy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * app_command.h
 * Purpose: The base class for AppConfig commands.
 */

#pragma once

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "./command.h"
#include "../../../apps/interfaces.h"


__CORE_COMMANDS_BEGIN__

/// Derived commands must have a constructor
///	with pointer to apps::IAppConfig and pointer
/// to conf::Settings parameters.
class AppCommand : public Command
{
protected:
	apps::IAppConfig* app_config;

	AppCommand(
		apps::IAppConfig* app_cfg, conf::Settings* settings, const std::string& cmd_name, const std::string& help
	);
};

__CORE_COMMANDS_END__
