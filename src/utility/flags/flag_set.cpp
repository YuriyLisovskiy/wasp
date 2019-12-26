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
 * An implementation of flag_set.h.
 */

#include "./flag_set.h"


__FLAGS_BEGIN__

FlagSet::flag_parser::flag_parser(int argc, char** argv, size_t parse_from, bool is_verbose)
{
	std::string last_arg;
	bool is_received = true;
	if (is_verbose)
	{
		std::cout << "Parsing the command line arguments..." << std::endl;
	}

	for (size_t i = parse_from; i < argc; i++)
	{
		std::string token = argv[i];
		if (str::starts_with(token, "--"))
		{
			if (!is_received)
			{
				this->flags[last_arg] = "true";
			}

			last_arg = str::ltrim(std::move(token), '-');
			is_received = false;
		}
		else
		{
			is_received = true;
			this->flags[last_arg] = token;
		}
	}

	if (is_verbose){
		std::cout << "---------------------------" << std::endl;
		std::cout << "Parsed arguments:" << std::endl;
		for(auto& a : this->flags)
		{
			std::cout << "    " << a.first << ": " << a.second << std::endl;
		}
		std::cout << "---------------------------" << std::endl;
	}
}

bool FlagSet::flag_parser::exists(const std::string& label)
{
	return this->flags.find(label) != this->flags.end();
}

std::string FlagSet::flag_parser::get_arg(const std::string& label)
{
	return this->flags[label];
}

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
	flag_parser fp(argc, argv, parse_from, is_verbose);
	for (auto& flag : this->_flags)
	{
		if (fp.exists(flag.first))
		{
			flag.second->_data = fp.get_arg(flag.first);
		}
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

BoolFlag* FlagSet::make_bool(const std::string& label, const std::string& help)
{
	auto* flag = new BoolFlag(label, help);
	this->_flags[label] = flag;
	return flag;
}

__FLAGS_END__
