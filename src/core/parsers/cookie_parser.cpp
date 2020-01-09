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
 * An implementation of cookie_parser.h
 */

#include "./cookie_parser.h"


__CORE_INTERNAL_BEGIN__


std::map<std::string, std::string>* cookie_parser::parse(const std::string& content)
{
	auto* result = new std::map<std::string, std::string>();
	if (content.empty())
	{
		return result;
	}

	auto begin = content.begin();
	auto end = content.end();
	std::string cookie_key, cookie_value;
	cookie_parser::state st = cookie_parser::state::req_key;
	while (begin != end)
	{
		char input = *begin++;
		switch (st)
		{
			case cookie_parser::state::req_key:
				if (input == '=')
				{
					st = cookie_parser::state::req_val;
				}
				else if (input != ' ')
				{
					cookie_key.push_back(input);
				}
				break;
			case cookie_parser::state::req_val:
				if (input == ';')
				{
					(*result)[cookie_key] = cookie_value;
					cookie_key.clear();
					cookie_value.clear();
					st = cookie_parser::state::req_key;
				}
				else if (input != ' ')
				{
					cookie_value.push_back(input);
				}
				break;
		}
	}

	(*result)[cookie_key] = cookie_value;
	return result;
}

__CORE_INTERNAL_END__
