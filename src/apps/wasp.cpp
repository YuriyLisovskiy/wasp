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
	for (auto& command : this->_settings->COMMANDS)
	{
		this->_help_message += command.second->usage() + '\n';
	}

	if (!this->_help_message.empty())
	{
		this->_help_message = "Available commands:\n\n" + this->_help_message;
	}
	else
	{
		this->_help_message = "Application has not commands.\n\n";
	}
}

void WaspApplication::execute_from_command_line(int argc, char** argv)
{
	if (argc > 1)
	{
		try
		{
			if (this->_settings->COMMANDS.find(argv[1]) != this->_settings->COMMANDS.end())
			{
				this->_settings->COMMANDS[argv[1]]->run_from_argv(argc, argv);
			}
			else
			{
				std::cout << "Command \"" << argv[1] << "\" is not found.\n\n";
			}
		}
		catch (const core::CommandError& exc)
		{
			this->_settings->LOGGER->error(exc.what(), exc.line(), exc.function(), exc.file());
		}
		catch (const core::InterruptException& exc)
		{
			this->_settings->LOGGER->error(exc.what(), exc.line(), exc.function(), exc.file());
		}
	}
	else
	{
		std::cout << this->_help_message;
	}
}

__APPS_END__
