/**
 * http/internal/cookie_parser.cpp
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include "./cookie_parser.h"

// Base libraries.
#include <xalwart.base/string_utils.h>

// Framework libraries.
#include "./utilities.h"


__HTTP_INTERNAL_BEGIN__

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

bool is_cookie_name_valid(const std::string& raw)
{
	if (raw.empty())
	{
		return false;
	}

	return std::find_if(raw.begin(), raw.end(), util::is_not_token) == std::end(raw);
}

std::vector<Cookie> read_cookies(const std::vector<std::string>& lines, const std::string& filter)
{
	std::vector<Cookie> cookies;
	for (auto line : lines)
	{
		util::trim_ascii_space(line);
		std::string part;
		while (!line.empty())
		{
			auto split_index = line.find(';');
			if (split_index != std::string::npos && split_index > 0)
			{
				part = line.substr(0, split_index);
				line = line.substr(split_index + 1);
			}
			else
			{
				part = line;
				line = "";
			}

			util::trim_ascii_space(part);
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

			cookies.push_back(Cookie(name, value));
		}
	}

	return cookies;
}

__HTTP_INTERNAL_END__
