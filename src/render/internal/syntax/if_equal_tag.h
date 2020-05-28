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
 * render/internal/syntax/if_equal_tag.h
 *
 * Purpose:
 *  TODO: implement docs for if_equal_tag.h
 */

#pragma once

// Module definitions.
#include "./_def_.h"

// Framework modules.
#include "../parser.h"


__SYNTAX_BEGIN__

const std::string TAG_NAME_IF_EQUAL = "if_equal";
const std::string TAG_NAME_IF_NOT_EQUAL = "if_not_equal";

struct if_equal_node : public node
{
	std::shared_ptr<FilterExpression> var1;
	std::shared_ptr<FilterExpression> var2;

	std::shared_ptr<node_list> node_list_true;
	std::shared_ptr<node_list> node_list_false;

	bool negate;

	std::string render(IContext* ctx) override;
};

extern std::shared_ptr<internal::node> do_if_equal(
	internal::parser* parser,
	internal::token_t& token,
	bool negate
);

// Output the contents of the block if the two arguments'
// string representations equal each other.
//
// Note: for user-defined objects' comparison implement custom
// filters.
//
// Examples:
//
// {% if_equal(user.id, comment.user_id) %}
//      ...
// {% end_if_equal %}
//
// {% if_not_equal(user.id, comment.user_id) %}
//      ...
// {% else %}
//      ...
// {% end_if_not_equal %}
extern std::function<std::shared_ptr<internal::node>(
	internal::parser*, internal::token_t& token
)> make_if_equal_tag();

// Output the contents of the block if the two arguments'
// string representations are not equal.
// See if_equal.
extern std::function<std::shared_ptr<internal::node>(
	internal::parser*, internal::token_t& token
)> make_if_not_equal_tag();

__SYNTAX_END__
