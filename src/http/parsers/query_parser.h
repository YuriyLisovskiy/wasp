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
 * query_parser definition.
 * TODO: write docs.
 */

#ifndef WASP_QUERY_PARSER_H
#define WASP_QUERY_PARSER_H

#include "../../globals.h"
#include "../request_parameters.h"
#include "../../collections/dict.h"
#include "../../collections/multi_dict.h"


__INTERNAL_BEGIN__

class QueryParser
{
private:
	enum ParserState
	{
		Key,
		Val
	};
	Dict<std::string, std::string> _dict;
	MultiValueDict<std::string, std::string> _multiDict;

	void _appendParameter(const std::string& key, const std::string& value);

public:
	explicit QueryParser();
	RequestParameters<std::string, std::string>* parse(const std::string& content);
	void reset();
};

__INTERNAL_END__


#endif // WASP_QUERY_PARSER_H
