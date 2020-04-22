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
 * An implementation of render/internal/syntax/if_equal_tag.h
 */

// Header.
#include "./if_equal_tag.h"

// Framework modules.
#include "../utility.h"


__SYNTAX_BEGIN__

std::string if_equal_node::render(IContext* ctx)
{
	auto val1 = this->var1->resolve(ctx);
	auto val2 = this->var2->resolve(ctx);
	if ((this->negate && val1 != val2) || (!this->negate && val1 == val2))
	{
		return this->node_list_true->render(ctx);
	}

	return this->node_list_false->render(ctx);
}

std::shared_ptr<internal::node> do_if_equal(
	internal::parser* parser,
	internal::token_t& token,
	bool negate
)
{
	std::vector<token_t> params;
	size_t curr_pos;
	if (!split_params(token.content, token.line_no, curr_pos, params))
	{
		parser::invalid_syntax(token, curr_pos);
	}

	std::string tag_name = negate ? TAG_NAME_IF_NOT_EQUAL : TAG_NAME_IF_EQUAL;
	if (params.size() != 2)
	{
		parser::throw_error(
			tag_name + " tag takes two argument", token
		);
	}

	auto res_node = std::make_shared<if_equal_node>();

	auto end_tag = "end_" + tag_name;
	res_node->node_list_true = parser->parse({"else", end_tag});
	auto next_token = parser->next_token();
	if (next_token.content == "else")
	{
		res_node->node_list_false = parser->parse({end_tag});
		parser->del_first_token();
	}
	else
	{
		res_node->node_list_false = std::make_shared<node_list>();
	}

	res_node->var1 = parser->compile_filter(params[0]);
	res_node->var2 = parser->compile_filter(params[1]);
	res_node->negate = negate;
	return res_node;
}

std::function<std::shared_ptr<internal::node>(
	internal::parser*, internal::token_t& token
)> make_if_equal_tag()
{
	return [](
		internal::parser* parser,
		internal::token_t& token
	) -> std::shared_ptr<internal::node>
	{
		return do_if_equal(parser, token, false);
	};
}

std::function<std::shared_ptr<internal::node>(
	internal::parser*, internal::token_t& token
)> make_if_not_equal_tag()
{
	return [](
		internal::parser* parser,
		internal::token_t& token
	) -> std::shared_ptr<internal::node>
	{
		return do_if_equal(parser, token, true);
	};
}

__SYNTAX_END__
