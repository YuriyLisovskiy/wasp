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
 * CookieParser implementation.
 * TODO: write docs.
 */

#include "cookie_parser.h"


__INTERNAL_BEGIN__


std::map<std::string, std::string>* CookieParser::parseRequestCookies(const std::string& content)
{
	auto* result = new std::map<std::string, std::string>();
	if (content.empty())
	{
		return result;
	}

	auto begin = content.begin();
	auto end = content.end();
	std::string cookieKey;
	std::string cookieValue;

	ReqCookieParserState state = ReqCookieParserState::Key;

	while (begin != end)
	{
		char input = *begin++;
		switch (state)
		{
			case ReqCookieParserState::Key:
				if (input == '=')
				{
					state = ReqCookieParserState::Val;
				}
				else if (input != ' ')
				{
					cookieKey.push_back(input);
				}
				break;
			case ReqCookieParserState::Val:
				if (input == ';')
				{
					(*result)[cookieKey] = cookieValue;
					cookieKey.clear();
					cookieValue.clear();
					state = ReqCookieParserState::Key;
				}
				else if (input != ' ')
				{
					cookieValue.push_back(input);
				}
				break;
		}
	}
	(*result)[cookieKey] = cookieValue;

	return result;
}

std::map<std::string, std::string>* CookieParser::parseResponseCookies(const std::string& content)
{
	auto* result = new std::map<std::string, std::string>();
	if (content.empty())
	{
		return result;
	}

	// TODO

	return result;
}

__INTERNAL_END__
