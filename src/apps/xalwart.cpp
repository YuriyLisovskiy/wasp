/**
 * apps/xalwart.cpp
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 */

#include "./xalwart.h"

// C++ libraries.
#include <iostream>

// Core libraries.
#include <xalwart.core/path.h>

// Framework libraries.
#include "../management/app.h"


__APPS_BEGIN__

MainApplication::MainApplication(conf::Settings* settings)
{
	core::InterruptException::initialize();
	if (!settings)
	{
		throw core::ImproperlyConfigured(
			"Setting must be configured in order to use the application."
		);
	}

	this->_settings = settings;
	this->_settings->init();
	this->_settings->prepare();
	this->_perform_checks();

	this->_setup_commands();

	for (auto& command : this->_commands)
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

void MainApplication::execute(int argc, char** argv)
{
	if (argc > 1)
	{
		try
		{
			if (this->_commands.find(argv[1]) != this->_commands.end())
			{
				this->_commands[argv[1]]->run_from_argv(argc, argv);
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

void MainApplication::_setup_commands()
{
	// Retrieve commands from INSTALLED_APPS and
	// check if apps' commands do not override
	// settings commands, if not, then append them
	// to settings.COMMANDS
	for (auto& installed_app : this->_settings->INSTALLED_APPS)
	{
		this->_extend_settings_commands_or_error(
			installed_app->get_commands(),
			[installed_app](const std::string& cmd_name) -> std::string {
				return "App with name '" + installed_app->get_name() +
					"' overrides settings.COMMANDS with '" + cmd_name + "' command";
			}
		);
	}

	auto default_commands = management::CoreManagementAppConfig(
		this->_settings
	).get_commands();

	// Check if user-defined commands do not override
	// default commands, if not, append them to settings.COMMANDS
	this->_extend_settings_commands_or_error(
		default_commands,
		[](const std::string& cmd_name) -> std::string {
			return "Attempting to override '" + cmd_name + "' command which may produce an undefined behaviour.";
		}
	);
}

void MainApplication::_extend_settings_commands_or_error(
	const std::vector<std::shared_ptr<cmd::BaseCommand>>& from,
	const std::function<std::string(const std::string& cmd_name)>& err_fn
)
{
	for (auto& command : from)
	{
		if (std::find_if(
			this->_commands.begin(),
			this->_commands.end(),
			[command](const std::pair<
				std::string, std::shared_ptr<cmd::BaseCommand>
			>& pair) -> bool {
				return command->name() == pair.first;
			}
		) != this->_commands.end())
		{
			this->_settings->LOGGER->warning(err_fn(command->name()));
		}

		this->_commands[command->name()] = command;
	}
}

void MainApplication::_perform_checks()
{
	std::cout << "Performing checks...";
	if (!this->_settings->LOGGER)
	{
		throw core::ImproperlyConfigured("LOGGER instance must be configured");
	}

	size_t err_count = 0;
	if (!this->_settings->TEMPLATES_ENGINE)
	{
		this->_settings->LOGGER->error(
			"TEMPLATES_ENGINE instance must be configured in order to use the application."
		);
		err_count++;
	}

	if (this->_settings->MIDDLEWARE.empty())
	{
		this->_settings->LOGGER->warning("You have not added any middleware.");
	}

	if (this->_settings->BASE_DIR.empty())
	{
		this->_settings->LOGGER->error(
			"BASE_DIR must not be empty in order to use the application."
		);
		err_count++;
	}
	else if (!core::path::exists(this->_settings->BASE_DIR))
	{
		this->_settings->LOGGER->error(
			"BASE_DIR must exist in order to use the application."
		);
		err_count++;
	}

	if (this->_settings->INSTALLED_APPS.empty())
	{
		this->_settings->LOGGER->warning(
			"You have not added any app to INSTALLED_APPS setting."
		);
	}

	if (this->_settings->SECRET_KEY.empty())
	{
		this->_settings->LOGGER->error(
			"SECRET_KEY must be set in order to use the application."
		);
		err_count++;
	}

	for (size_t i = 0; i < this->_settings->INSTALLED_APPS.size(); i++)
	{
		if (!this->_settings->INSTALLED_APPS[i]->is_initialized())
		{
			this->_settings->LOGGER->error("Application is not initialized.");
			err_count++;
			break;
		}
	}

	if (err_count > 0)
	{
		throw core::ImproperlyConfigured(
			"System check identified " + std::to_string(err_count) + " issues."
		);
	}

	this->_settings->LOGGER->print(" Done.");
}

__APPS_END__
