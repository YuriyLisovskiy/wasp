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
 * An implementation of render/library/syntax/static_tag.h
 */

#include "./static_tag.h"

// Framework modules.
#include "./parse_var_name.h"
#include "../utility.h"


__SYNTAX_BEGIN__

std::string static_node::render(IContext* ctx)
{
	std::string result;
	if (this->path)
	{
		result = core::path::join(this->prefix, this->path->resolve(ctx));
	}

	if (!this->var_name.empty())
	{
		ctx->push_var(
			this->var_name,
			std::make_shared<core::types::Value<std::string>>(result)
		);
		result = "";
	}

	return result;
}

std::function<std::shared_ptr<internal::node>(
	internal::parser*, internal::token_t& token
)> make_static_tag(const std::string& name, const std::string& prefix)
{
	return [name, prefix](
		internal::parser* parser,
		internal::token_t& token
	) -> std::shared_ptr<internal::node>
	{
		std::vector<token_t> params;
		size_t curr_pos;
		if (!split_params(token.content, token.line_no, curr_pos, params))
		{
			parser::invalid_syntax(token, curr_pos);
		}

		if (params.size() != 1)
		{
			parser::throw_error("'" + name + "' tag takes exactly one argument", token);
		}

		std::string var_name;

		// parse variable name after tag (function) call
		if (!parse_var_name(token, var_name, curr_pos, false))
		{
			parser::invalid_syntax(token, curr_pos - 1);
		}

		auto node = std::make_shared<static_node>();
		node->path = parser->compile_filter(params[0]);
		node->prefix = prefix;
		node->var_name = var_name;

		return node;
	};
}

__SYNTAX_END__
