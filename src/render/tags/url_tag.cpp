/**
 * render/tags/url_tag.cpp
 *
 * Copyright (c) 2020-2021 Yuriy Lisovskiy
 */

#include "./url_tag.h"

// Base libraries.
#include <xalwart.base/types/string.h>

// Render libraries.
#include <xalwart.render/syntax/utility.h>

// Framework libraries.
#include "../../utility/functional.h"


__RENDER_TAGS_BEGIN__

std::string UrlNode::render(abc::IContext* ctx)
{
	using FilterExpressionPointer = std::shared_ptr<syntax::FilterExpression>;
	auto built_url = this->pattern->build(
		util::fn::map<FilterExpressionPointer, std::string>(
			this->params, [ctx](const FilterExpressionPointer& p) -> std::string {
				auto p_var = p->resolve(ctx);
				return p_var ? p_var->__str__() : "";
			}
		)
	);

	if (!this->var_name.empty())
	{
		ctx->push_var(this->var_name, std::make_shared<types::String>(built_url));
		return "";
	}

	return built_url;
}

std::function<std::shared_ptr<syntax::node>(
	syntax::parser*, syntax::token_t& token
)> make_url_tag(const std::vector<std::shared_ptr<urls::IPattern>>& patterns)
{
	return [patterns](
		syntax::parser* parser,
		syntax::token_t& token
	) -> std::shared_ptr<syntax::node>
	{
		std::vector<syntax::token_t> params;
		size_t curr_pos;
		if (!split_params(token.content, token.line_no, curr_pos, params))
		{
			syntax::parser::invalid_syntax(token, curr_pos);
		}

		if (params.empty())
		{
			syntax::parser::throw_error("'url' tag takes, at least, one argument", token);
		}

		std::string var_name;

		// parse variable name after tag (function) call
		if (!syntax::parse_var_name(token, var_name, curr_pos, false))
		{
			syntax::parser::invalid_syntax(token, curr_pos - 1);
		}

		auto url_name = params[0].content;
		if (!syntax::trim_quotes(url_name))
		{
			syntax::parser::throw_error("first parameter of 'url' tag must be constant string", token);
		}

		for (const auto& url_p : patterns)
		{
			if (url_p->get_name() == url_name)
			{
				auto node = std::make_shared<UrlNode>();
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

		syntax::parser::throw_error("url pattern '" + url_name + "' does not exist", token);
		return nullptr;
	};
}

__RENDER_TAGS_END__
