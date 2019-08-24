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
 * query_parser implementation.
 * TODO: write docs.
 */

#include "query_parser.h"


__INTERNAL_BEGIN__

QueryParser::QueryParser() : _dict(true), _multiDict(true)
{
}

RequestParameters<std::string, std::string>* QueryParser::parse(const std::string& content)
{
	if (content.empty())
	{
		return new RequestParameters<std::string, std::string>();
	}

	auto begin = content.begin();
	auto end = content.end();
	std::string itemKey;
	std::string itemValue;

	ParserState state = ParserState::Key;

	while (begin != end)
	{
		char input = *begin++;
		switch (state)
		{
			case ParserState::Key:
				if (input == '=')
				{
					state = ParserState::Val;
				}
				else
				{
					itemKey.push_back(input);
				}
				break;
			case ParserState::Val:
				if (input == '&')
				{
					this->_appendParameter(itemKey, itemValue);
					itemKey.clear();
					itemValue.clear();
					state = ParserState::Key;
				}
				else
				{
					itemValue.push_back(input);
				}
				break;
		}
	}
	this->_appendParameter(itemKey, itemValue);

	return new RequestParameters<std::string, std::string>(this->_dict, this->_multiDict);
}

void QueryParser::_appendParameter(const std::string& key, const std::string& value)
{
	if (!this->_dict.contains(key))
	{
		this->_dict.set(key, value);
	}
	this->_multiDict.append(key, value);
}

void QueryParser::reset()
{
	this->_dict.clear();
	this->_multiDict.clear();
}

__INTERNAL_END__
