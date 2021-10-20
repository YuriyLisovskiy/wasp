/**
 * commands/command.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * Base class for modules' commands.
 */

#pragma once

// STL libraries.
#include <memory>
#include <string>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "./abstract_command.h"
#include "./flags/default.h"
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

	void add_flags() override;

	void validate() const override;

	bool handle() override;

private:
	std::shared_ptr<flags::BoolFlag> _print_help_flag = nullptr;
	std::shared_ptr<flags::BoolFlag> _use_colors_flag = nullptr;

	inline void _use_colors_for_logging(bool use_colors)
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
