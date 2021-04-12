/**
 * core/parsers/query_parser.cpp
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 */

#include "./query_parser.h"


__PARSERS_BEGIN__

void url_query_parser::parse(const std::string& content)
{
	if (content.empty())
	{
		return;
	}

	auto begin = content.begin();
	auto end = content.end();
	std::string item_key, item_value;
	url_query_parser::state st = url_query_parser::state::s_key;
	while (begin != end)
	{
		char input = *begin++;
		switch (st)
		{
			case url_query_parser::state::s_key:
				if (input == '=')
				{
					st = url_query_parser::state::s_val;
				}
				else
				{
					item_key.push_back(input);
				}
				break;
			case url_query_parser::state::s_val:
				if (input == '&')
				{
					this->append_parameter(item_key, item_value);
					item_key.clear();
					item_value.clear();
					st = url_query_parser::state::s_key;
				}
				else
				{
					item_value.push_back(input);
				}
				break;
		}
	}

	this->append_parameter(item_key, item_value);
}

void url_query_parser::append_parameter(const std::string& key, const std::string& value)
{
	if (!this->dict.contains(key))
	{
		this->dict.set(key, value);
	}

	this->multi_dict.append(key, value);
}

__PARSERS_END__
