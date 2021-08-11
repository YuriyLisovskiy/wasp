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
	internal::ArgumentsParser ap(argc, argv, parse_from, is_verbose);
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
			"flag provided but not defined: " + ap.flags.begin()->first, _ERROR_DETAILS_
		);
	}
}

std::string FlagSet::usage(const std::string& indent) const
{
	if (!this->_usage.empty())
	{
		return this->_usage;
	}

	size_t max_len = 0;
	for (auto& flag : this->_flags)
	{
		auto new_len = flag.first.second.size();
		if (new_len > max_len)
		{
			max_len = new_len;
		}
	}

	std::string usage;
	for (auto& flag : this->_flags)
	{
		std::string ind(max_len - flag.first.second.size(), ' ');
		usage.append(indent + flag.first.first);
		usage.append(", ");
		usage.append(flag.first.second);
		usage.append(ind + "   " + flag.second->usage() + "\n");
	}

	return str::rtrim(usage, '\n');
}

std::shared_ptr<Uint16Flag> FlagSet::make_uint16(
	const std::string& shorthand, const std::string& name, uint16_t default_val, const std::string& usage
)
{
	auto flag = std::make_shared<Uint16Flag>(shorthand, name, usage, default_val);
	if (this->_flags.find({flag->shorthand(), flag->name()}) != this->_flags.end())
	{
		throw ValueError(
			"flag <" + flag->shorthand() + ", " + flag->name() + "> already exists", _ERROR_DETAILS_
		);
	}

	this->_flags[{flag->shorthand(), flag->name()}] = flag;
	return flag;
}

std::shared_ptr<UnsignedLongFlag> FlagSet::make_unsigned_long(
	const std::string& shorthand, const std::string& name, unsigned long default_val, const std::string& usage
)
{
	auto flag = std::make_shared<UnsignedLongFlag>(shorthand, name, usage, default_val);
	if (this->_flags.find({flag->shorthand(), flag->name()}) != this->_flags.end())
	{
		throw ValueError(
			"flag <" + flag->shorthand() + ", " + flag->name() + "> already exists", _ERROR_DETAILS_
		);
	}

	this->_flags[{flag->shorthand(), flag->name()}] = flag;
	return flag;
}

std::shared_ptr<LongIntFlag> FlagSet::make_long(
	const std::string& shorthand, const std::string& name, long default_val, const std::string& usage
)
{
	auto flag = std::make_shared<LongIntFlag>(shorthand, name, usage, default_val);
	if (this->_flags.find({flag->shorthand(), flag->name()}) != this->_flags.end())
	{
		throw ValueError(
			"flag <" + flag->shorthand() + ", " + flag->name() + "> already exists", _ERROR_DETAILS_
		);
	}

	this->_flags[{flag->shorthand(), flag->name()}] = flag;
	return flag;
}

std::shared_ptr<DoubleFlag> FlagSet::make_double(
	const std::string& shorthand, const std::string& name, double default_val, const std::string& usage
)
{
	auto flag = std::make_shared<DoubleFlag>(shorthand, name, usage, default_val);
	if (this->_flags.find({flag->shorthand(), flag->name()}) != this->_flags.end())
	{
		throw ValueError(
			"flag <" + flag->shorthand() + ", " + flag->name() + "> already exists", _ERROR_DETAILS_
		);
	}

	this->_flags[{flag->shorthand(), flag->name()}] = flag;
	return flag;
}

std::shared_ptr<StringFlag> FlagSet::make_string(
	const std::string& shorthand, const std::string& name, const std::string& default_val, const std::string& usage
)
{
	auto flag = std::make_shared<StringFlag>(shorthand, name, usage, default_val);
	if (this->_flags.find({flag->shorthand(), flag->name()}) != this->_flags.end())
	{
		throw ValueError(
			"flag <" + flag->shorthand() + ", " + flag->name() + "> already exists", _ERROR_DETAILS_
		);
	}

	this->_flags[{flag->shorthand(), flag->name()}] = flag;
	return flag;
}

std::shared_ptr<BoolFlag> FlagSet::make_bool(
	const std::string& shorthand, const std::string& name, bool default_val, const std::string& usage
)
{
	auto flag = std::make_shared<BoolFlag>(shorthand, name, usage, default_val);
	if (this->_flags.find({flag->shorthand(), flag->name()}) != this->_flags.end())
	{
		throw ValueError(
			"flag <" + flag->shorthand() + ", " + flag->name() + "> already exists", _ERROR_DETAILS_
		);
	}

	this->_flags[{flag->shorthand(), flag->name()}] = flag;
	return flag;
}

__CORE_FLAGS_END__
