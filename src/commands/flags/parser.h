/**
 * commands/flags/parser.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * Command line arguments parser.
 */

#pragma once

// C++ libraries.
#include <map>
#include <string>

// Module definitions.
#include "./_def_.h"


__COMMANDS_FLAGS_BEGIN__

// TESTME: ArgumentsParser
// TODO: docs for 'ArgumentsParser'
struct ArgumentsParser
{
	std::map<std::string, std::string> flags;

	explicit ArgumentsParser(int argc, char** argv, size_t parse_from=1, bool is_verbose=false);

	[[nodiscard]]
	inline bool exists(const std::string& label) const
	{
		return this->flags.find(label) != this->flags.end();
	}

	[[nodiscard]]
	inline std::string get_arg(const std::string& label) const
	{
		return this->flags.at(label);
	}

	inline void remove_arg(const std::string& label)
	{
		if (this->exists(label))
		{
			this->flags.erase(label);
		}
	}
};

__COMMANDS_FLAGS_END__
