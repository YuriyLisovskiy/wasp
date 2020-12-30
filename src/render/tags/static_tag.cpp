/**
 * render/tags/static_tag.cpp
 *
 * Copyright (c) 2020 Yuriy Lisovskiy
 */

#include "./static_tag.h"

// Core libraries.
#include <xalwart.core/path.h>
#include <xalwart.core/types/value.h>

// Render libraries.
#include <xalwart.render/internal/syntax/parse_var_name.h>
#include <xalwart.render/internal/utility.h>


__TAGS_BEGIN__

std::string static_node::render(IContext* ctx)
{
	std::string result;
	if (this->path)
	{
		auto path_var = this->path->resolve(ctx);
		result = path::join(this->prefix, path_var ? path_var->__str__() : "");
	}

	if (!this->var_name.empty())
	{
		ctx->push_var(
			this->var_name,
			std::make_shared<types::Value<std::string>>(result)
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
		if (!syntax::parse_var_name(token, var_name, curr_pos, false))
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

__TAGS_END__
