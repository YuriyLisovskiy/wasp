/**
 * core/flags/flag_set.cpp
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 */

#include "./flag_set.h"

// Base libraries.
#include <xalwart.base/exceptions.h>

// Framework libraries.
#include "./parser.h"


__CORE_FLAGS_BEGIN__

void FlagSet::parse(int argc, char** argv, size_t parse_from, bool is_verbose)
{
	internal::args_parser ap(argc, argv, parse_from, is_verbose);
	for (auto& flag : this->_flags)
	{
		std::string key;
		if (ap.exists(flag.first.first))
		{
			key = flag.first.first;
		}
		else if (ap.exists(flag.first.second))
		{
			key = flag.first.second;
		}

		if (!key.empty())
		{
			flag.second->_data = ap.get_arg(key);
			ap.remove_arg(key);
		}
	}

	if (!ap.flags.empty())
	{
		throw ArgumentError(
			"flag provided but not defined: " + ap.flags.begin()->first,
			_ERROR_DETAILS_
		);
	}
}

std::string FlagSet::usage(const std::string& indent) const
{
	if (!this->_usage.empty())
	{
		return this->_usage;
	}

	std::string usage = indent + "Usage options:";
	for (auto& flag : this->_flags)
	{
		usage.append("\n" + indent + "  -" + flag.first.first + ", --" + flag.first.second + "\t" + flag.second->usage());
	}

	return usage;
}

std::shared_ptr<Uint16Flag> FlagSet::make_uint16(
	const std::string& short_label, const std::string& label, uint16_t default_val, const std::string& usage
)
{
	auto flag = std::make_shared<Uint16Flag>(short_label, label, usage, default_val);
	this->_flags[{short_label, label}] = flag;
	return flag;
}

std::shared_ptr<UnsignedLongFlag> FlagSet::make_unsigned_long(
	const std::string& short_label, const std::string& label, unsigned long default_val, const std::string& usage
)
{
	auto flag = std::make_shared<UnsignedLongFlag>(short_label, label, usage, default_val);
	this->_flags[{short_label, label}] = flag;
	return flag;
}

std::shared_ptr<LongIntFlag> FlagSet::make_long(
	const std::string& short_label, const std::string& label, long default_val, const std::string& usage
)
{
	auto flag = std::make_shared<LongIntFlag>(short_label, label, usage, default_val);
	this->_flags[{short_label, label}] = flag;
	return flag;
}

std::shared_ptr<DoubleFlag> FlagSet::make_double(
	const std::string& short_label, const std::string& label, double default_val, const std::string& usage
)
{
	auto flag = std::make_shared<DoubleFlag>(short_label, label, usage, default_val);
	this->_flags[{short_label, label}] = flag;
	return flag;
}

std::shared_ptr<StringFlag> FlagSet::make_string(
	const std::string& short_label, const std::string& label, const std::string& default_val, const std::string& usage
)
{
	auto flag = std::make_shared<StringFlag>(short_label, label, usage, default_val);
	this->_flags[{short_label, label}] = flag;
	return flag;
}

std::shared_ptr<BoolFlag> FlagSet::make_bool(
	const std::string& short_label, const std::string& label, bool default_val, const std::string& usage
)
{
	auto flag = std::make_shared<BoolFlag>(short_label, label, usage, default_val);
	this->_flags[{short_label, label}] = flag;
	return flag;
}

__CORE_FLAGS_END__
