/**
 * commands/base.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * The base class from which all commands ultimately derive.
 */

#pragma once

// C++ libraries.
#include <string>

// Base libraries.
#include <xalwart.base/kwargs.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "../core/flags/flag_set.h"


__COMMANDS_BEGIN__

/// Use this class if you want access to all of the mechanisms which
/// parse the command-line arguments and work out what code to call in
/// response; if you don't need to change any of that behavior,
/// consider using one of the subclasses defined in this file.
class BaseCommand
{
protected:
	std::shared_ptr<core::flags::FlagSet> flag_set;
	bool is_created;

	// Index of item in argv to parse from.
	size_t parse_from;

	std::string help;
	std::string label;

	explicit BaseCommand(const std::string& cmd_name, const std::string& help);

	/// Creates flags if they are not created yet.
	void create_flags();

	/// Override in child class to add more commands.
	virtual inline void add_flags()
	{
	}

	/// Validates arguments before processing the command.
	virtual inline void validate()
	{
	}

	/// The actual logic of the command. Subclasses must implement
	/// this method.
	virtual void handle() = 0;

public:

	// Returns command flags.
	virtual inline Kwargs get_kwargs()
	{
		return Kwargs();
	}

	/// Returns usage based on flag_set.
	std::string usage();

	/// Returns full command's name.
	inline std::string name()
	{
		return this->label;
	}

	/// Creates flags if they are not created yet,
	/// parses argv and runs handle() method.
	void run_from_argv(int argc, char** argv, bool is_verbose = false);
};

__COMMANDS_END__
