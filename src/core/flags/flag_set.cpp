/**
 * core/flags/flag_set.cpp
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 */

#include "./flag_set.h"

// Core libraries.
#include <xalwart.core/exceptions.h>

// Framework libraries.
#include "./parser.h"


__FLAGS_BEGIN__

FlagSet::FlagSet(const std::string& name, const std::string& usage)
{
	this->_name = name;
	this->_usage = usage;
}

void FlagSet::parse(int argc, char** argv, size_t parse_from, bool is_verbose)
{
	internal::args_parser ap(argc, argv, parse_from, is_verbose);
	for (auto& flag : this->_flags)
	{
		if (ap.exists(flag.first))
		{
			flag.second->_data = ap.get_arg(flag.first);
			ap.remove_arg(flag.first);
		}
	}

	if (!ap.flags.empty())
	{
		throw core::ArgumentError("flag provided but not defined: " + ap.flags.begin()->first);
	}
}

std::string FlagSet::usage(const std::string& indent)
{
	if (!this->_usage.empty())
	{
		return this->_usage;
	}

	std::string usage = indent + "Help options:";
	for (auto& flag : this->_flags)
	{
		usage.append("\n" + indent + "  --" + flag.first + "\t" + flag.second->usage());
	}

	return usage;
}

std::shared_ptr<LongIntFlag> FlagSet::make_long(
	const std::string& label, long default_val, const std::string& help
)
{
	auto flag = std::make_shared<LongIntFlag>(label, help, default_val);
	this->_flags[label] = flag;
	return flag;
}

std::shared_ptr<DoubleFlag> FlagSet::make_double(
	const std::string& label, double default_val, const std::string& help
)
{
	auto flag = std::make_shared<DoubleFlag>(label, help, default_val);
	this->_flags[label] = flag;
	return flag;
}

std::shared_ptr<StringFlag> FlagSet::make_string(
	const std::string& label, const std::string& default_val, const std::string& help
)
{
	auto flag = std::make_shared<StringFlag>(label, help, default_val);
	this->_flags[label] = flag;
	return flag;
}

std::shared_ptr<BoolFlag> FlagSet::make_bool(
	const std::string& label, bool default_val, const std::string& help
)
{
	auto flag = std::make_shared<BoolFlag>(label, help, default_val);
	this->_flags[label] = flag;
	return flag;
}

__FLAGS_END__
