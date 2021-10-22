/**
 * commands/abstract_command.cpp
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 */

#include "./abstract_command.h"

// STL
#include <algorithm>


__COMMANDS_BEGIN__

std::string AbstractCommand::usage()
{
	this->create_flags();
	bool has_commands = !this->_subcommands.empty();
	auto usage_message = this->help_message() +
		"\n\nUsage:\n  application " + this->name() + (has_commands ? " [command]" : " [flags]");
	if (has_commands)
	{
		usage_message += "\n\nCommands:";
		std::vector<size_t> sizes;
		sizes.reserve(this->_subcommands.size());
		std::transform(
			this->_subcommands.begin(), this->_subcommands.end(), std::back_inserter(sizes),
			[](const auto& pair) -> size_t { return pair.second ? pair.second->name().size() : 0; }
		);

		size_t max_command_length = *std::max_element(sizes.begin(), sizes.end());
		for (const auto& subcommand : this->_subcommands)
		{
			if (subcommand.second)
			{
				std::string indent(max_command_length - subcommand.second->name().size(), ' ');
				usage_message += "\n  " + subcommand.second->name() + indent + "  " +
					subcommand.second->help_message();
			}
		}

		usage_message +=  + "\n\nUse \"application " + this->name() +
			" [command] --help\" for more information about a command.";
	}
	else
	{
		usage_message += "\n\nFlags:\n" + this->flag_set->usage("  ");
	}

	return usage_message;
}

void AbstractCommand::run_from_argv(int argc, char** argv, size_t start_from, bool is_verbose)
{
	this->create_flags();
	if (start_from < argc)
	{
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
	else
	{
		this->flag_set->parse(argc, argv, start_from, is_verbose);
		this->validate();
		this->handle();
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
