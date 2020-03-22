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
 * An implementation of render/library/syntax/url_tag.h
 */

#include "./url_tag.h"

// Framework modules.
#include "../../../core/string/str.h"

__SYNTAX_BEGIN__



std::function<std::shared_ptr<internal::node>(
	internal::parser*, internal::token_t& token
)> make_url_tag(const std::vector<urls::UrlPattern>& patterns)
{
	return [](
		internal::parser* parser,
		internal::token_t& token
	) -> std::shared_ptr<internal::node>
	{
		// left bracket position
		size_t pos = token.content.find('(');
		if (pos == std::string::npos)
		{
			internal::parser::invalid_syntax(token, token.content.size());
		}

		// TODO: implement make_url_tag(const std::vector<urls::UrlPattern>& patterns)
		return nullptr;
	};
}

__SYNTAX_END__
