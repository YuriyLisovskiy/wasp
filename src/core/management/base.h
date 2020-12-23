/**
 * core/management/base.h
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 *
 * Purpose:
 * 	The base class from which all management commands
 * 	ultimately derive.
 */

#pragma once

// Module definitions.
#include "../_def_.h"

// Framework libraries.
#include "../../core/flags/flag_set.h"


__CORE_BEGIN__

/// Use this class if you want access to all of the mechanisms which
/// parse the command-line arguments and work out what code to call in
/// response; if you don't need to change any of that behavior,
/// consider using one of the subclasses defined in this file.
class BaseCommand
{
protected:
	flags::FlagSet* _flag_set;
	bool _is_created;
	size_t _parse_from;
	std::string _help;
	std::string _name;

	explicit BaseCommand(const std::string& cmd_name, const std::string& help);

	/// Creates flags if they are not created yet.
	void create_flags();

	/// Override in child class to add more commands.
	virtual void add_flags();

	virtual void validate();

	/// The actual logic of the command. Subclasses must implement
	/// this method.
	virtual void handle() = 0;

public:
	virtual ~BaseCommand();

	/// Returns usage based on flag_set.
	std::string usage();

	/// Returns full command's name.
	std::string name();

	/// Creates flags if they are not created yet,
	/// parses argv and runs handle() method.
	void run_from_argv(int argc, char** argv, bool is_verbose = false);
};

__CORE_END__
