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
 * An implementation of core/flags/parser.h
 */

#include "./parser.h"

// C++ libraries.
#include <iostream>

// Framework modules.
#include <xalwart.core/strings.h>


__FLAGS_INTERNAL_BEGIN__

args_parser::args_parser(int argc, char** argv, size_t parse_from, bool is_verbose)
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
		if (core::str::starts_with(token, "--"))
		{
			if (!is_received)
			{
				this->flags[last_arg] = "true";
			}

			auto param = core::str::split(token, '=');
			if (param.size() > 1)
			{
				core::str::ltrim(param[0], "-");
				auto val = core::str::join(param.begin() + 1, param.end(), "");
				this->flags[param[0]] = val;
			}
			else
			{
				last_arg = token;
				core::str::ltrim(last_arg, "-");
				is_received = false;
			}
		}
		else
		{
			is_received = true;
			this->flags[last_arg] = token;
		}
	}

	if (!is_received)
	{
		this->flags[last_arg] = "true";
	}

	if (is_verbose)
	{
		std::cout << "---------------------------" << std::endl;
		std::cout << "Parsed arguments:" << std::endl;
		for(auto& a : this->flags)
		{
			std::cout << "    " << a.first << ": " << a.second << std::endl;
		}

		std::cout << "---------------------------" << std::endl;
	}
}

bool args_parser::exists(const std::string& label)
{
	return this->flags.find(label) != this->flags.end();
}

std::string args_parser::get_arg(const std::string& label)
{
	return this->flags[label];
}

void args_parser::remove_arg(const std::string& label)
{
	if (this->exists(label))
	{
		this->flags.erase(label);
	}
}

__FLAGS_INTERNAL_END__
