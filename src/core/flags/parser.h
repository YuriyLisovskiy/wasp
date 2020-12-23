/**
 * core/flags/parser.h
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 *
 * Purpose: parses command line arguments.
 */

#pragma once

// C++ libraries.
#include <map>

#ifdef _MSC_VER
#include <string>
#endif

// Module definitions.
#include "./_def_.h"


__FLAGS_INTERNAL_BEGIN__

struct args_parser
{
	std::map<std::string, std::string> flags;

	explicit args_parser(int argc, char** argv, size_t parse_from = 1, bool is_verbose = false);
	bool exists(const std::string& label);
	std::string get_arg(const std::string& label);
	void remove_arg(const std::string& label);
};

__FLAGS_INTERNAL_END__
