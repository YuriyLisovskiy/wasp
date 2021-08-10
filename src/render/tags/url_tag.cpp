/**
 * render/tags/url_tag.cpp
 *
 * Copyright (c) 2020-2021 Yuriy Lisovskiy
 */

#include "./url_tag.h"

// C++ libraries.
#include <iostream>

// Base libraries.
#include <xalwart.base/types/string.h>

// Render libraries.
#include <xalwart.render/syntax/parse_var_name.h>
#include <xalwart.render/syntax/utility.h>

// Framework libraries.
#include "../../utility/functional.h"


__RENDER_TAGS_BEGIN__

std::string url_node::render(abc::IContext* ctx)
{
	using Fe = std::shared_ptr<internal::FilterExpression>;
	auto built_url = this->pattern->build(
		util::fn::map<Fe, std::string>(this->params, [ctx](const Fe& p) -> std::string {
			auto p_var = p->resolve(ctx);
			return p_var ? p_var->__str__() : "";
		})
	);

	if (!this->var_name.empty())
	{
		ctx->push_var(this->var_name, std::make_shared<types::String>(built_url));
		return "";
	}

	return built_url;
}

std::function<std::shared_ptr<internal::node>(
	internal::parser*, internal::token_t& token
)> make_url_tag(const std::vector<std::shared_ptr<urls::IPattern>>& patterns)
{
	return [patterns](
		internal::parser* parser,
		internal::token_t& token
	) -> std::shared_ptr<internal::node>
	{
		std::vector<internal::token_t> params;
		size_t curr_pos;
		if (!split_params(token.content, token.line_no, curr_pos, params))
		{
			internal::parser::invalid_syntax(token, curr_pos);
		}

		if (params.empty())
		{
			internal::parser::throw_error("'url' tag takes, at least, one argument", token);
		}

		std::string var_name;

		// parse variable name after tag (function) call
		if (!internal::syntax::parse_var_name(token, var_name, curr_pos, false))
		{
			internal::parser::invalid_syntax(token, curr_pos - 1);
		}

		auto url_name = params[0].content;
		if (!internal::trim_quotes(url_name))
		{
			internal::parser::throw_error("first parameter of 'url' tag must be constant string", token);
		}

		for (const auto& url_p : patterns)
		{
			if (url_p->get_name() == url_name)
			{
				auto node = std::make_shared<url_node>();
				if (params.size() > 1)
				{
					for (size_t i = 1; i < params.size(); i++)
					{
						node->params.push_back(parser->compile_filter(params[i]));
					}
				}

				node->pattern = url_p;
				node->var_name = var_name;
				return node;
			}
		}

		internal::parser::throw_error("url pattern '" + url_name + "' does not exist", token);
		return nullptr;
	};
}

__RENDER_TAGS_END__
