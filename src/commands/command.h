/**
 * commands/command.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * Base class for modules' commands.
 */

#pragma once

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "./abstract_command.h"
#include "../conf/settings.h"


__COMMANDS_BEGIN__

// TESTME: Command
// TODO: docs for 'Command'
class Command : public AbstractCommand
{
protected:
	conf::Settings* settings;
	const conf::IModuleConfig* module_config;

	Command(
		const conf::IModuleConfig* module,
		conf::Settings* settings,
		const std::string& cmd_name,
		const std::string& help
	);

	inline void use_colors_for_logging(bool use_colors)
	{
		if (use_colors)
		{
			this->settings->LOGGER->enable_colors();
		}
		else
		{
			this->settings->LOGGER->disable_colors();
		}
	}
};

__COMMANDS_END__
