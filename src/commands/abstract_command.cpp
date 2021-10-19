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

void AbstractCommand::run_from_argv(int argc, char** argv, size_t start_from, bool is_verbose)
{
	this->create_flags();
	std::string first_token = argv[start_from];
	if (first_token.starts_with('-'))
	{
		this->flag_set->parse(argc, argv, start_from, is_verbose);
		this->validate();
		this->handle();
	}
	else
	{
		// subcommand detected
		if (this->_subcommands.contains(first_token))
		{
			this->_subcommands.at(first_token)->run_from_argv(argc, argv, start_from + 1, is_verbose);
		}
		else
		{
			throw CommandError(
				"Command '" + this->name() + "' does not contain subcommand with name '" + first_token + "'",
				_ERROR_DETAILS_
			);
		}
	}
}

AbstractCommand::AbstractCommand(const std::string& cmd_name, const std::string& help)
{
	this->flag_set = std::make_shared<flags::FlagSet>(cmd_name);
	this->is_created = false;
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

void AbstractCommand::add_subcommand(std::shared_ptr<AbstractCommand> command)
{
	require_non_null(command.get(), "command is nullptr", _ERROR_DETAILS_);
	auto name = command->name();
	if (this->_subcommands.contains(name))
	{
		this->_subcommands[name] = std::move(command);
	}
	else
	{
		this->_subcommands.insert(std::make_pair(name, std::move(command)));
	}
}

__COMMANDS_END__
