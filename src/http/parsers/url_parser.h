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

#ifndef WASP_HTTP_PARSERS_URL_PARSER_H
#define WASP_HTTP_PARSERS_URL_PARSER_H

#include <string>
#include <cstdlib>
#include <cstdint>
#include <cassert>
#include <cstdlib>

#include "../../globals.h"
#include "../../core/exceptions.h"


__INTERNAL_BEGIN__

struct url_parser
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
		s_port_or_password,
		s_port,
		s_path,
		s_query,
		s_fragment
	};

	bool is_parsed;
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
	static bool is_unreserved(char ch);
};

__INTERNAL_END__


#endif // WASP_HTTP_PARSERS_URL_PARSER_H
