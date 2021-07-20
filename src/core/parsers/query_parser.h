/**
 * core/parsers/query_parser.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * URL's query parser.
 */

#pragma once

// C++ libraries.
#include <string>

// Base libraries.
#include <xalwart.base/string_utils.h>
#include <xalwart.base/collections/multi_dict.h>

// Module definitions.
#include "./_def_.h"


__CORE_PARSERS_BEGIN__

// TESTME: url_query_parser
struct url_query_parser final
{
	enum state
	{
		s_key,
		s_val
	};

	collections::Dict<std::string, std::string> dict;
	collections::MultiValueDict<std::string, std::string> multi_dict;

	// Default constructor.
	url_query_parser() = default;

	// Appends parsed parameter to the result.
	//
	// `key`: name of the parameter.
	// `value`: actual value of the parameter.
	void append_parameter(const std::string& key, const std::string& value);

	// Parses URL's query. Splits query string into key-value pairs.
	//
	// `data`: an actual query.
	void parse(const std::string& data);
};

__CORE_PARSERS_END__
