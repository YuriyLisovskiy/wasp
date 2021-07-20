/**
 * core/flags/parser.cpp
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 */

#include "./parser.h"

// C++ libraries.
#include <iostream>

// Base libraries.
#include <xalwart.base/string_utils.h>


__CORE_FLAGS_INTERNAL_BEGIN__

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
		if (token.starts_with("--"))
		{
			if (!is_received)
			{
				this->flags[last_arg] = "true";
			}

			auto param = str::split(token, '=');
			if (param.size() > 1)
			{
				param[0] = str::ltrim(param[0], "-");
				auto val = str::join("", param.begin() + 1, param.end());
				this->flags[param[0]] = val;
			}
			else
			{
				last_arg = token;
				last_arg = str::ltrim(last_arg, "-");
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

__CORE_FLAGS_INTERNAL_END__
