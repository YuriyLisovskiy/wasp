/**
 * core/parsers/cookie_parser.h
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 *
 * Purpose: parses cookies from http request.
 */

#pragma once

// C++ libraries.
#include <map>
#include <string>

// Module definitions.
#include "../_def_.h"


__PARSERS_BEGIN__

struct cookie_parser final
{
	enum state
	{
		req_key,
		req_val
	};

	std::map<std::string, std::string> result;

	void parse(const std::string& content);
};

__PARSERS_END__
