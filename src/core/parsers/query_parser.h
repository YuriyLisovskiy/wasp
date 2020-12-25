/**
 * core/parsers/query_parser.h
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 *
 * Purpose: parses URL's query.
 */

#pragma once

// C++ libraries.
#include <string>

// Core libraries.
#include <xalwart.core/collections/multi_dict.h>

// Module definitions.
#include "../_def_.h"


__CORE_INTERNAL_BEGIN__

struct query_parser final
{
	enum state
	{
		s_key,
		s_val
	};

	collections::Dict<std::string, std::string> dict;
	collections::MultiValueDict<std::string, std::string> multi_dict;

	query_parser() = default;
	void append_parameter(const std::string& key, const std::string& value);
	void parse(const std::string& data);
};

__CORE_INTERNAL_END__
