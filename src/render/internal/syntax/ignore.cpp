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
 * An implementation of render/internal/syntax/ignore.h
 */

#include "./ignore.h"


__SYNTAX_BEGIN__

std::function<std::shared_ptr<internal::node>(
	internal::parser*, internal::token_t& token
)> make_ignore_tag()
{
	return [](
		internal::parser* parser,
		internal::token_t& token
	) -> std::shared_ptr<internal::node>
	{
		parser->skip_past({"end_" + TAG_NAME_IGNORE});
		return std::make_shared<ignore_node>();
	};
}

__SYNTAX_END__
