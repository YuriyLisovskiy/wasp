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
#include <xalwart.core/string.h>
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

	collections::Dict<std::string, xw::string> dict;
	collections::MultiValueDict<std::string, xw::string> multi_dict;

	query_parser() = default;
	void append_parameter(const std::string& key, const xw::string& value);
	void parse(const xw::string& data);
};

__CORE_INTERNAL_END__
