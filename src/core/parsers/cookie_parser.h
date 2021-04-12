/**
 * core/parsers/cookie_parser.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * HTTP cookies' parser.
 */

#pragma once

// C++ libraries.
#include <map>
#include <string>

// Module definitions.
#include "../_def_.h"


__PARSERS_BEGIN__

// TESTME: cookie_parser
struct cookie_parser final
{
	enum state
	{
		req_key,
		req_val
	};

	std::map<std::string, std::string> result;

	// Splits cookie header to map.
	//
	// `content`: input string content.
	void parse(const std::string& content);
};

__PARSERS_END__
