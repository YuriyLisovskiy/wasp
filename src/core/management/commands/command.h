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
 * command.h
 * Purpose: base class for all commands
 */

#pragma once

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "../base.h"
#include "../../../conf/settings.h"


__CORE_COMMANDS_BEGIN__

class Command : public BaseCommand
{
protected:
	conf::Settings* settings;

	Command(
		conf::Settings* settings, const std::string& cmd_name, const std::string& help
	);
};

__CORE_COMMANDS_END__
