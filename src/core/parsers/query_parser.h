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

/**
 * query_parser.h
 * Purpose: parses url's query.
 */

#ifndef WASP_CORE_PARSERS_QUERY_PARSER_H
#define WASP_CORE_PARSERS_QUERY_PARSER_H

#include "../../globals.h"
#include "../../http/request.h"
#include "../../collections/dict.h"
#include "../../collections/multi_dict.h"


__INTERNAL_BEGIN__

struct query_parser final
{
	enum state
	{
		s_key,
		s_val
	};

	Dict<std::string, std::string>* dict;
	MultiValueDict<std::string, std::string>* multi_dict;

	explicit query_parser();
	~query_parser();
	void append_parameter(const std::string& key, const std::string& value);
	void parse(const std::string& data);
};

__INTERNAL_END__


#endif // WASP_CORE_PARSERS_QUERY_PARSER_H
