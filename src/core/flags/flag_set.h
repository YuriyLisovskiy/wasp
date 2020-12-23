/**
 * core/flags/flag_set.h
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 *
 * Purpose: represents set of flags which can be parsed from char** argv
 */

#pragma once

// C++ libraries.
#include <map>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "./flags.h"


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
