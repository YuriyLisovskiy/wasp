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

// C++ libraries.
#include <vector>

#include "./url_tag.h"

// Framework modules.
#include "../utility.h"
#include "../../../utils/functional.h"

__SYNTAX_BEGIN__

std::string url_node::render(IContext* ctx)
{
	using Fe = std::shared_ptr<FilterExpression>;
	auto built_url = this->pattern->build(
		utils::fn::map<Fe, std::string>(this->params, [ctx](const Fe& p) -> std::string {
			return p->resolve(ctx);
		})
	);

	if (!this->var_name.empty())
	{
		ctx->push_var(
			this->var_name,
			std::make_shared<core::types::Value<std::string>>(built_url)
		);
		return "";
	}

	return built_url;
}

void parse(
	internal::token_t& token,
	std::vector<token_t>& params,
	std::string& var_name
);

std::function<std::shared_ptr<internal::node>(
	internal::parser*, internal::token_t& token
)> make_url_tag(const std::vector<std::shared_ptr<urls::UrlPattern>>& patterns)
{
	return [patterns](
		internal::parser* parser,
		internal::token_t& token
	) -> std::shared_ptr<internal::node>
	{
		std::string var_name;
		std::vector<token_t> params;
		parse(token, params, var_name);
		if (params.empty())
		{
			parser::throw_error("'url' tag takes, at least, one argument", token);
		}

		auto url_name = params[0].content;
		if (
			core::str::starts_with(url_name, "'") &&
			core::str::ends_with(url_name, "'")
		)
		{
			core::str::trim(url_name, "'");
		}
		else if (
			core::str::starts_with(url_name, "\"") &&
			core::str::ends_with(url_name, "\"")
		)
		{
			core::str::trim(url_name, "\"");
		}
		else
		{
			parser::throw_error("first parameter of 'url' tag must be string", token);
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

		parser::throw_error("url pattern '" + url_name + "' does not exist", token);
		return nullptr;
	};
}

enum parser_state
{
	s_space_1, s_space_2, s_dash, s_var_name
};

void parse(
	internal::token_t& token,
	std::vector<token_t>& params,
	std::string& var_name
)
{
	// left bracket position
	size_t pos = token.content.find('(');
	if (pos == std::string::npos)
	{
		internal::parser::invalid_syntax(token, token.content.size());
	}

	size_t curr_pos;
	if (!split_params(token.content.substr(pos), token.line_no, curr_pos, params))
	{
		parser::invalid_syntax(token, curr_pos);
	}

	// Parse var assigning, i.e. url('u_name', ...) -> var_name
	curr_pos += pos;
	parser_state st = parser_state::s_space_1;
	while (curr_pos < token.content.size())
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
					parser::invalid_syntax(token, curr_pos - 1);
				}
				break;
			case parser_state::s_dash:
				if (ch == '>')
				{
					st = parser_state::s_space_2;
				}
				else
				{
					parser::invalid_syntax(token, curr_pos - 1);
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
					parser::invalid_syntax(token, curr_pos - 1);
				}
				break;
			case parser_state::s_var_name:
				if (is_var_char(ch))
				{
					var_name += ch;
				}
				else
				{
					parser::invalid_syntax(token, curr_pos - 1);
				}
				break;
		}
	}
}

__SYNTAX_END__
