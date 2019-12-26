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
 * An implementation of base.h.
 */

#include "./base.h"


__CORE_COMMANDS_BEGIN__

BaseCommand::BaseCommand(const std::string& cmd_name, const std::string& help)
{
	this->_flag_set = new flags::FlagSet(cmd_name);
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
	this->_flag_set->parse(argc, argv, is_verbose);
	this->handle();
}

__CORE_COMMANDS_END__
