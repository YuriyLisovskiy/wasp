/**
 * commands/app_command.cpp
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 */

#include "./app_command.h"


__COMMANDS_BEGIN__

AppCommand::AppCommand(
	apps::IAppConfig* app_cfg, conf::Settings* settings, const std::string& cmd_name, const std::string& help
) : BaseCommand(cmd_name, help), app_config(app_cfg), settings(settings)
{
	if (!this->app_config)
	{
		throw core::ImproperlyConfigured(
			"ModuleCommand: module config must be instantiated in order to use the application",
			_ERROR_DETAILS_
		);
	}

	if (!this->settings)
	{
		throw core::ImproperlyConfigured(
			"ModuleCommand: settings must be instantiated in order to use the application",
			_ERROR_DETAILS_
		);
	}
}

__COMMANDS_END__
