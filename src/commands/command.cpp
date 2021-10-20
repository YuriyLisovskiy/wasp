/**
 * commands/command.cpp
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 */

#include "./command.h"

// STL libraries.
#include <iostream>


__COMMANDS_BEGIN__

Command::Command(
	const conf::IModuleConfig* module_cfg,
	conf::Settings* settings,
	const std::string& cmd_name,
	const std::string& help
) : AbstractCommand(cmd_name, help), module_config(module_cfg), settings(settings)
{
	if (!this->module_config)
	{
		throw NullPointerException("xw::cmd::ModuleCommand: module_cfg is nullptr", _ERROR_DETAILS_);
	}

	if (!this->settings)
	{
		throw NullPointerException("xw::cmd::ModuleCommand: settings is nullptr", _ERROR_DETAILS_);
	}
}

void Command::add_flags()
{
	this->_use_colors_flag = this->flag_set->make_bool(
		"c", "colors", true, "Enables colors for logging in terminal"
	);
	this->_print_help_flag = this->flag_set->make_bool(
		"h", "help", false, "Print usage"
	);
}

void Command::validate() const
{
	if (!this->_print_help_flag->valid())
	{
		throw CommandError("Got invalid value of boolean flag: -h, --help");
	}

	if (!this->_use_colors_flag->valid())
	{
		throw CommandError("Got invalid value of boolean flag: -c, --colors");
	}
}

bool Command::handle()
{
	if (this->_print_help_flag->get())
	{
		std::cout << this->usage() << '\n';
		return true;
	}

	this->_use_colors_for_logging(this->_use_colors_flag->get());
	return false;
}

__COMMANDS_END__
