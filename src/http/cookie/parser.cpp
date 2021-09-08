/**
 * http/cookie/parser.cpp
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include "./parser.h"

// Base libraries.
#include <xalwart.base/string_utils.h>


__HTTP_BEGIN__

std::pair<std::string, bool> parse_cookie_value(std::string raw_val, bool allow_double_quote)
{
	if (allow_double_quote && raw_val.size() > 1 && raw_val.front() == '"' && raw_val.back() == '"')
	{
		raw_val = str::trim(raw_val, '"');
	}

	for (char b : raw_val)
	{
		if (!valid_cookie_value_byte(b))
		{
			return {"", false};
		}
	}

	return {raw_val, true};
}

std::vector<Cookie> parse_cookies(std::string raw_cookie, const std::string& filter)
{
	std::vector<Cookie> cookies;
	raw_cookie = str::trim_func(raw_cookie, is_ascii_space);
	std::string part;
	while (!raw_cookie.empty())
	{
		auto split_index = raw_cookie.find(';');
		if (split_index != std::string::npos && split_index > 0)
		{
			part = raw_cookie.substr(0, split_index);
			raw_cookie = raw_cookie.substr(split_index + 1);
		}
		else
		{
			part = raw_cookie;
			raw_cookie = "";
		}

		part = str::trim_func(part, is_ascii_space);
		if (part.empty())
		{
			continue;
		}

		std::string name = part, val;
		auto j = part.find('=');
		if (j != std::string::npos)
		{
			val = name.substr(j + 1);
			name = name.substr(0, j);
		}

		if (!is_cookie_name_valid(name))
		{
			continue;
		}

		if (!filter.empty() && filter != name)
		{
			continue;
		}

		auto [value, ok] = parse_cookie_value(val, true);
		if (!ok)
		{
			continue;
		}

		cookies.emplace_back(name, value);
	}

	return cookies;
}

__HTTP_END__
