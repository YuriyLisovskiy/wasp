/*
 * Copyright (c) 2020 Yuriy Lisovskiy
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
 * An implementation of render/internal/utility.h
 */

#include "./utility.h"

// Framework modules.
#include "../../core/string/str.h"


__RENDER_INTERNAL_BEGIN__

bool split_params(
	const std::string& content,
	size_t line_no,
	size_t& curr_pos,
	std::vector<token_t>& params
)
{
	// left bracket position
	curr_pos = content.find('(');
	if (curr_pos == std::string::npos)
	{
		curr_pos = 0;
		return false;
	}

	if (content.substr(curr_pos).size() < 2)
	{
		return false;
	}

	if (content[curr_pos++] != '(')
	{
		return false;
	}

	if (content[curr_pos] == ')')
	{
		return true;
	}

	auto end_pos = content.size();
	std::string last_param;
	size_t open_brackets = 0;
	bool triggered_comma = false;
	while (curr_pos != end_pos)
	{
		char ch = content[curr_pos++];
		switch (ch)
		{
			case '(':
				open_brackets++;
				last_param += ch;
				triggered_comma = false;
				break;
			case ')':
				if (open_brackets == 0)
				{
					if (triggered_comma)
					{
						return false;
					}

					token_t token;
					core::str::trim(last_param);
					token.content = last_param;
					token.line_no = line_no;
					token.position = {curr_pos - last_param.size(), curr_pos};
					params.push_back(token);
					return true;
				}

				open_brackets--;
				last_param += ch;
				break;
			case ',':
				if (open_brackets == 0)
				{
					token_t token;
					core::str::trim(last_param);
					token.content = last_param;
					token.line_no = line_no;
					token.position = {curr_pos - last_param.size(), curr_pos};
					params.push_back(token);
					last_param.clear();
					triggered_comma = true;
				}
				else
				{
					last_param += ch;
				}
				break;
			default:
				last_param += ch;
				triggered_comma = false;
		}
	}

	return false;
}

bool is_var_char(char ch)
{
	return std::isalnum(ch) || ch == '_';
}

bool is_var_char_begin(char ch)
{
	return std::isalpha(ch) || ch == '_';
}

bool trim_quotes(std::string& str)
{
	if (
		core::str::starts_with(str, "'") &&
		core::str::ends_with(str, "'")
	)
	{
		core::str::trim(str, "'");
	}
	else if (
		core::str::starts_with(str, "\"") &&
		core::str::ends_with(str, "\"")
	)
	{
		core::str::trim(str, "\"");
	}
	else
	{
		return false;
	}

	return true;
}

__RENDER_INTERNAL_END__
