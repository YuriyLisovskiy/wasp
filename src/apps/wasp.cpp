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
	if (!this->_settings->LOGGER)
	{
		throw core::ImproperlyConfigured("logger can not be nullptr", _ERROR_DETAILS_);
	}

	this->_settings->init();
	this->_settings->overwrite();

	this->setup_commands();

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

void WaspApplication::setup_commands()
{
	// Retrieve commands from INSTALLED_APPS and
	// check if apps' commands do not override
	// settings commands, if not, then append them
	// to settings.COMMANDS
	for (auto& installed_app : this->_settings->INSTALLED_APPS)
	{
		this->extend_settings_commands_or_error(
			installed_app->get_commands(),
			[installed_app](const std::string& cmd_name) -> std::string {
				return "App with name '" + installed_app->get_name() +
					"' overrides settings.COMMANDS with '" + cmd_name + "' command";
			}
		);
	}

	auto default_commands = core::internal::CoreManagementAppConfig(
		this->_settings
	).get_commands();

	// Check if user-defined commands do not override
	// default commands, if not, append them to settings.COMMANDS
	this->extend_settings_commands_or_error(
		default_commands,
		[](const std::string& cmd_name) -> std::string {
			return "Attempting to override '" + cmd_name + "' command which is forbidden.";
		}
	);
}

void WaspApplication::extend_settings_commands_or_error(
	const std::vector<core::BaseCommand*>& from,
	const std::function<std::string(const std::string& cmd_name)>& err_fn
)
{
	for (auto& command : from)
	{
		if (std::find_if(
			this->_settings->COMMANDS.begin(),
			this->_settings->COMMANDS.end(),
			[command](const std::pair<std::string, core::BaseCommand*>& pair) -> bool {
				return command->name() == pair.first;
			}
		) != this->_settings->COMMANDS.end())
		{
			throw core::ImproperlyConfigured(err_fn(command->name()), _ERROR_DETAILS_);
		}

		this->_settings->COMMANDS[command->name()] = command;
	}
}

__APPS_END__
