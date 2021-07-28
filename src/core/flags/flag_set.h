/**
 * core/flags/flag_set.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * Set of flags which can be parsed from char** argv
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
	std::map<std::pair<std::string, std::string>, std::shared_ptr<Flag>> _flags;

public:
	inline explicit FlagSet(std::string name, std::string usage="") :
		_name(std::move(name)), _usage(std::move(usage))
	{
	}

	void parse(int argc, char** argv, size_t parse_from=1, bool is_verbose=false);

	[[nodiscard]]
	inline std::string name() const
	{
		return this->_name;
	}

	[[nodiscard]]
	std::string usage(const std::string& indent="") const;

	std::shared_ptr<Uint16Flag> make_uint16(
		const std::string& short_label, const std::string& label, uint16_t default_val, const std::string& usage
	);

	std::shared_ptr<UnsignedLongFlag> make_unsigned_long(
		const std::string& short_label, const std::string& label, unsigned long default_val, const std::string& usage
	);

	std::shared_ptr<LongIntFlag> make_long(
		const std::string& short_label, const std::string& label, long default_val, const std::string& usage
	);

	std::shared_ptr<DoubleFlag> make_double(
		const std::string& short_label, const std::string& label, double default_val, const std::string& usage
	);

	std::shared_ptr<StringFlag> make_string(
		const std::string& short_label, const std::string& label, const std::string& default_val, const std::string& usage
	);

	std::shared_ptr<BoolFlag> make_bool(
		const std::string& short_label, const std::string& label, bool default_val, const std::string& usage
	);
};

__CORE_FLAGS_END__
