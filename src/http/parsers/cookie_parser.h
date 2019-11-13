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

/*
 * CookieParser definition.
 * TODO: write docs.
 */

#ifndef WASP_HTTP_PARSERS_COOKIE_PARSER_H
#define WASP_HTTP_PARSERS_COOKIE_PARSER_H

#include <map>
#include <string>

#include "../../globals.h"


__INTERNAL_BEGIN__

struct cookie_parser final
{
	enum req_state
	{
		req_key,
		req_val
	};

	enum resp_state
	{

	};

	static std::map<std::string, std::string>* parse_req_cookies(const std::string& content);
	static std::map<std::string, std::string>* parse_resp_cookies(const std::string& content);
};

__INTERNAL_END__


#endif // WASP_HTTP_PARSERS_COOKIE_PARSER_H
