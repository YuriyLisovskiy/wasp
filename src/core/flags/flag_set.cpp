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
 * An implementation of core/flags/flag_set.h
 */

#include "./flag_set.h"

// Framework modules.
#include "./parser.h"
#include "../exceptions.h"


__FLAGS_BEGIN__

FlagSet::FlagSet(const std::string& name, const std::string& usage)
{
	this->_name = name;
	this->_usage = usage;
}

FlagSet::~FlagSet()
{
	for (auto& flag : this->_flags)
	{
		delete flag.second;
	}
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

LongIntFlag* FlagSet::make_long(
	const std::string& label, long default_val, const std::string& help
)
{
	auto* flag = new LongIntFlag(label, help, default_val);
	this->_flags[label] = flag;
	return flag;
}

DoubleFlag* FlagSet::make_double(
	const std::string& label, double default_val, const std::string& help
)
{
	auto* flag = new DoubleFlag(label, help, default_val);
	this->_flags[label] = flag;
	return flag;
}

StringFlag* FlagSet::make_string(
	const std::string& label, const std::string& default_val, const std::string& help
)
{
	auto* flag = new StringFlag(label, help, default_val);
	this->_flags[label] = flag;
	return flag;
}

BoolFlag* FlagSet::make_bool(const std::string& label, bool default_val, const std::string& help)
{
	auto* flag = new BoolFlag(label, help, default_val);
	this->_flags[label] = flag;
	return flag;
}

__FLAGS_END__
