/**
 * commands/abstract_command.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * The base class from which all commands ultimately derive.
 */

#pragma once

// C++ libraries.
#include <string>
#include <memory>

// Base libraries.
#include <xalwart.base/options.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "./flags/flag_set.h"


__COMMANDS_BEGIN__

// TESTME: AbstractCommand
// Use this class if you want access to all the mechanisms which
// parse the command-line arguments and work out what code to call in
// response; if you don't need to change any of that behavior,
// consider using one of the subclasses defined in this file.
class AbstractCommand
{
public:
	// Returns command flags.
	[[nodiscard]]
	virtual inline Options get_options() const
	{
		return {};
	}

	// Returns usage based on flag_set.
	std::string usage();

	// Returns help message for command.
	[[nodiscard]]
	inline std::string help_message() const
	{
		return this->help;
	}

	// Returns full command's name.
	[[nodiscard]]
	inline std::string name() const
	{
		return this->label;
	}

	void run_from_argv(int argc, char** argv, size_t start_from=2, bool is_verbose=false);

protected:
	std::shared_ptr<flags::FlagSet> flag_set;
	bool is_created;
	std::string help;
	std::string label;

	explicit AbstractCommand(const std::string& cmd_name, const std::string& help);

	// Create flags if they are not created yet.
	void create_flags();

	void add_subcommand(std::shared_ptr<AbstractCommand> command);

	// Override in child class to add more commands.
	virtual void add_flags() = 0;

	// Validates arguments before processing the command.
	virtual void validate() const = 0;

	// The actual logic of the command. Subclasses must implement
	// this method.
	//
	// Returns `true` if command were processed, `false` otherwise.
	virtual bool handle() = 0;

private:
	std::map<std::string, std::shared_ptr<AbstractCommand>> _subcommands;
};

template <typename T>
concept command_type = std::is_base_of_v<AbstractCommand, T>;

__COMMANDS_END__
