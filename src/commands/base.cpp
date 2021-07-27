/**
 * commands/base.cpp
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 */

#include "./base.h"


__COMMANDS_BEGIN__

BaseCommand::BaseCommand(const std::string& cmd_name, const std::string& help)
{
	this->flag_set = std::make_shared<core::flags::FlagSet>(cmd_name);
	this->is_created = false;
	this->parse_from = 2;
	this->help = help;
	this->label = cmd_name;
}

std::string BaseCommand::usage()
{
	this->create_flags();
	return this->label + ":\n" + this->help + "\n" + this->flag_set->usage("  ");
}

void BaseCommand::create_flags()
{
	if (!this->is_created)
	{
		this->add_flags();
		this->is_created = true;
	}
}

void BaseCommand::run_from_argv(int argc, char** argv, bool is_verbose)
{
	this->create_flags();
	this->flag_set->parse(argc, argv, this->parse_from, is_verbose);
	this->handle();
}

__COMMANDS_END__
