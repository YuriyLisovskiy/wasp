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


__RENDER_INTERNAL_BEGIN__

bool split_params(
	const std::string& content,
	size_t& curr_pos,
	std::vector<std::string>& params
)
{
	curr_pos = 0;
	if (content.size() < 2)
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
		char ch = content[curr_pos];
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

					params.push_back(last_param);
					return true;
				}

				open_brackets--;
				last_param += ch;
				break;
			case ',':
				if (open_brackets == 0)
				{
					params.push_back(last_param);
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

		curr_pos++;
	}

	return false;
}

__RENDER_INTERNAL_END__
