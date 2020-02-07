/*
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
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
 * core/flags/flag_set.h
 *
 * Purpose:
 * 	Represents set of flags which can be parsed from char** argv
 */

#pragma once

// C++ libraries.
#include <map>
#include <string>

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "./flags.h"
#include "./parser.h"


__FLAGS_BEGIN__

class FlagSet final
{
private:
	std::string _name;
	std::string _usage;
	std::map<std::string, Flag*> _flags;

public:
	explicit FlagSet(const std::string& name, const std::string& usage = "");
	~FlagSet();
	void parse(int argc, char** argv, size_t parse_from = 1, bool is_verbose = false);
	std::string usage(const std::string& indent = "");
	LongIntFlag* make_long(
		const std::string& label, long default_val, const std::string& help
	);
	DoubleFlag* make_double(
		const std::string& label, double default_val, const std::string& help
	);
	StringFlag* make_string(
		const std::string& label, const std::string& default_val, const std::string& help
	);
	BoolFlag* make_bool(const std::string& label, bool default_val, const std::string& help);
};

__FLAGS_END__
