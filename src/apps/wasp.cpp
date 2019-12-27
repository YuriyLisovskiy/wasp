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
 * An implementation of wasp.h.
 */

#include "./wasp.h"


__APPS_BEGIN__

WaspApplication::WaspApplication(conf::Settings* settings)
{
	this->_settings = settings;
	this->_settings->init();
	this->_settings->overwrite();
}

void WaspApplication::execute_from_command_line(int argc, char** argv)
{
	if (argc > 1)
	{
		try
		{
			if (this->_settings->COMMANDS.find(argv[1]) != this->_settings->COMMANDS.end())
			{
				auto command = this->_settings->COMMANDS[argv[1]];
				command->run_from_argv(argc, argv);
			}
		}
		catch (const core::CommandError& exc)
		{
			this->_settings->LOGGER->fatal(exc.what(), exc.line(), exc.function(), exc.file());
		}
	}
}

__APPS_END__
