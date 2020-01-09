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
 * parser.h
 * Purpose: parses command line arguments.
 */

#pragma once

// C++ libraries.
#include <map>
#include <string>
#include <iostream>

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "../string/str.h"


__FLAGS_INTERNAL_BEGIN__

struct args_parser
{
	std::map<std::string, std::string> flags;

	explicit args_parser(int argc, char** argv, size_t parse_from = 1, bool is_verbose = false);
	bool exists(const std::string& label);
	std::string get_arg(const std::string& label);
};

__FLAGS_INTERNAL_END__
