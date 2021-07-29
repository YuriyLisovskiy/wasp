/**
 * http/parsers/cookie_parser.cpp
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 */

#include "./cookie_parser.h"


__HTTP_INTERNAL_BEGIN__

void cookie_parser::parse(const std::string& content)
{
	if (content.empty())
	{
		return;
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
					this->result[cookie_key] = cookie_value;
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

	this->result[cookie_key] = cookie_value;
}

__HTTP_INTERNAL_END__
