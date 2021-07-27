/**
 * commands/command.cpp
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 */

#include "./command.h"


__COMMANDS_BEGIN__

Command::Command(
	conf::IModuleConfig* module_cfg,
	conf::Settings* settings,
	const std::string& cmd_name,
	const std::string& help
) : BaseCommand(cmd_name, help), module_config(module_cfg), settings(settings)
{
	if (!this->module_config)
	{
		throw NullPointerException(
			"cmd::ModuleCommand: module_cfg is nullptr", _ERROR_DETAILS_
		);
	}

	if (!this->settings)
	{
		throw NullPointerException(
			"cmd::ModuleCommand: settings is nullptr", _ERROR_DETAILS_
		);
	}
}

__COMMANDS_END__
