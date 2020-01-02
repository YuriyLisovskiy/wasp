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

#pragma once

#include "../../../src/apps/interface.h"
#include "../../../src/core/management/commands/app_command.h"


class HelloCommand : public wasp::core::cmd::AppCommand
{
public:
	HelloCommand(wasp::apps::IAppConfig* config, wasp::conf::Settings* settings)
		: AppCommand(config, settings, "hello", "Prints hello from MainApp")
	{
	}

	void handle() override
	{
		this->settings->LOGGER->print("Hello from MainApp!");
	}
};
