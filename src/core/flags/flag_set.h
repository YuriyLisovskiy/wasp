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
#include "./default.h"


__CORE_FLAGS_BEGIN__

class FlagSet final
{
private:
	std::string _name;
	std::string _usage;
	std::map<std::string, std::shared_ptr<Flag>> _flags;

public:
	explicit FlagSet(const std::string& name, const std::string& usage = "");
	void parse(int argc, char** argv, size_t parse_from = 1, bool is_verbose = false);
	std::string usage(const std::string& indent = "");
	std::shared_ptr<Uint16Flag> make_uint16(
		const std::string& label, uint16_t default_val, const std::string& help
	);
	std::shared_ptr<UnsignedLongFlag> make_unsigned_long(
		const std::string& label, unsigned long default_val, const std::string& help
	);
	std::shared_ptr<LongIntFlag> make_long(
		const std::string& label, long default_val, const std::string& help
	);
	std::shared_ptr<DoubleFlag> make_double(
		const std::string& label, double default_val, const std::string& help
	);
	std::shared_ptr<StringFlag> make_string(
		const std::string& label, const std::string& default_val, const std::string& help
	);
	std::shared_ptr<BoolFlag> make_bool(
		const std::string& label, bool default_val, const std::string& help
	);
};

__CORE_FLAGS_END__
