/**
 * commands/abstract_command.cpp
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 */

#include "./abstract_command.h"


__COMMANDS_BEGIN__

std::string AbstractCommand::usage()
{
	this->create_flags();
	return this->help_message() +
		"\n\nUsage:\n  application " + this->name() + " [flags]\n\n" +
		"Flags:\n" + this->flag_set->usage("  ");
}

void AbstractCommand::run_from_argv(int argc, char** argv, bool is_verbose)
{
	this->create_flags();
	this->flag_set->parse(argc, argv, this->parse_from, is_verbose);
	this->handle();
}

AbstractCommand::AbstractCommand(const std::string& cmd_name, const std::string& help)
{
	this->flag_set = std::make_shared<flags::FlagSet>(cmd_name);
	this->is_created = false;
	this->parse_from = 2;
	this->help = help;
	this->label = cmd_name;
}

void AbstractCommand::create_flags()
{
	if (!this->is_created)
	{
		this->add_flags();
		this->is_created = true;
	}
}

__COMMANDS_END__
