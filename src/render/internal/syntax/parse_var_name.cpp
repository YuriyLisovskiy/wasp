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
 * An implementation of render/internal/syntax/parse_var_name.h
 */

#include "./parse_var_name.h"

// Framework modules.
#include "../utility.h"


__SYNTAX_BEGIN__

enum parser_state
{
	s_space_1, s_space_2, s_dash, s_var_name
};

bool parse_var_name(
	internal::token_t& token,
	std::string& var_name,
	size_t& curr_pos,
	bool has_content_after_var
)
{
	parser_state st = parser_state::s_space_1;
	bool stop = false;
	bool result_state = true;
	while (curr_pos < token.content.size() && !stop)
	{
		char ch = token.content[curr_pos++];
		switch (st)
		{
			case parser_state::s_space_1:
				if (ch == '-')
				{
					st = parser_state::s_dash;
				}
				else if (!std::isspace(ch))
				{
					stop = true;
					result_state = false;
				}
				break;
			case parser_state::s_dash:
				if (ch == '>')
				{
					st = parser_state::s_space_2;
				}
				else
				{
					stop = true;
					result_state = false;
				}
				break;
			case parser_state::s_space_2:
				if (is_var_char_begin(ch))
				{
					st = parser_state::s_var_name;
					var_name += ch;
				}
				else if (!std::isspace(ch))
				{
					stop = true;
					result_state = false;
				}
				break;
			case parser_state::s_var_name:
				if (is_var_char(ch))
				{
					var_name += ch;
				}
				else
				{
					stop = true;
					if (!has_content_after_var)
					{
						result_state = false;
					}
				}
				break;
		}
	}

	return result_state;
}

__SYNTAX_END__
