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
 * core/parsers/url_parser.h
 *
 * Purpose: parses url from given string.
 */

#pragma once

// C++ libraries.
#include <string>
#include <cassert>

// Module definitions.
#include "../_def_.h"

// Wasp libraries.
#include "../string/str.h"
#include "../../core/exceptions.h"


__CORE_INTERNAL_BEGIN__

struct url_parser final
{
	enum state
	{
		s_scheme,
		s_first_slash,
		s_second_slash,
		s_username_or_hostname,
		s_password,
		s_hostname,
		s_ipv6_hostname,
		s_ipv6_hostname_end,
		s_port_or_password,
		s_port,
		s_path,
		s_query,
		s_fragment
	};

	bool is_parsed;
	bool is_reset;
	const char* last_err;
	long int err_line;
	const char* err_file;
	const char* err_func;

	std::string scheme;
	std::string username;
	std::string password;
	std::string hostname;
	std::string port;
	std::string path;
	std::string query;
	std::string fragment;
	uint16_t integer_port;

	url_parser();
	void parse(const std::string& str);
	void parse(const char* str);
	void reset();
	static bool is_unreserved(char ch);
	static bool is_ipv6_symbol(char ch);

	void set_err(const char* err, int line, const char* func, const char* file);
};

__CORE_INTERNAL_END__
