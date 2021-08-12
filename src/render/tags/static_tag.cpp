/**
 * render/tags/static_tag.cpp
 *
 * Copyright (c) 2020-2021 Yuriy Lisovskiy
 */

#include "./static_tag.h"

// Base libraries.
#include <xalwart.base/path.h>
#include <xalwart.base/types/string.h>

// Render libraries.
#include <xalwart.render/syntax/utility.h>


__RENDER_TAGS_BEGIN__

std::string StaticNode::render(abc::IContext* ctx)
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
			std::make_shared<types::String>(result)
		);
		result = "";
	}

	return result;
}

std::function<std::shared_ptr<syntax::Node>(
	syntax::Parser*, syntax::Token& token
)> make_static_tag(const std::string& name, const std::string& prefix)
{
	return [name, prefix](syntax::Parser* parser, syntax::Token& token) -> std::shared_ptr<syntax::Node>
	{
		std::vector<syntax::Token> params;
		size_t curr_pos;
		if (!split_params(token.content, token.line_no, curr_pos, params))
		{
			syntax::Parser::invalid_syntax(token, curr_pos);
		}

		if (params.size() != 1)
		{
			syntax::Parser::throw_error("'" + name + "' tag takes exactly one argument", token);
		}

		std::string var_name;

		// parse variable name after tag (function) call
		if (!syntax::parse_var_name(token, var_name, curr_pos, false))
		{
			syntax::Parser::invalid_syntax(token, curr_pos - 1);
		}

		auto node = std::make_shared<StaticNode>();
		node->path = parser->compile_filter(params[0]);
		node->prefix = prefix;
		node->var_name = var_name;

		return node;
	};
}

__RENDER_TAGS_END__
