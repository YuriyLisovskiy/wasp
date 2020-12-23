/**
 * commands/base.cpp
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 */

#include "./base.h"


__COMMANDS_BEGIN__

BaseCommand::BaseCommand(const std::string& cmd_name, const std::string& help)
{
	this->_flag_set = new core::flags::FlagSet(cmd_name);
	this->_is_created = false;
	this->_parse_from = 2;
	this->_help = help;
	this->_name = cmd_name;
}

BaseCommand::~BaseCommand()
{
	delete this->_flag_set;
}

std::string BaseCommand::usage()
{
	this->create_flags();
	return this->_name + ":\n" + this->_help + "\n" + this->_flag_set->usage("  ");
}

std::string BaseCommand::name()
{
	return this->_name;
}

void BaseCommand::create_flags()
{
	if (!this->_is_created)
	{
		this->add_flags();
		this->_is_created = true;
	}
}

void BaseCommand::add_flags()
{
}

void BaseCommand::validate()
{
}

void BaseCommand::run_from_argv(int argc, char** argv, bool is_verbose)
{
	this->create_flags();
	this->_flag_set->parse(argc, argv, this->_parse_from, is_verbose);
	this->handle();
}

__COMMANDS_END__
