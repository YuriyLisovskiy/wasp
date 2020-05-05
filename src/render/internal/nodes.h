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

#pragma once

// Module definitions.
#include "../_def_.h"

// Framework modules.
#include "./token.h"
#include "./filter_expr.h"


__RENDER_INTERNAL_BEGIN__

struct node
{
	bool must_be_first;
	token_t token;

	bool is_text_node;

	node();
	virtual std::string render(IContext* ctx);
};

struct text_node : public node
{
	std::string text;

	text_node();
	explicit text_node(const std::string& s);
	std::string render(IContext* ctx) override;
};

struct variable_node : public node
{
	std::shared_ptr<FilterExpression> filter_expr;

	explicit variable_node(
		std::shared_ptr<FilterExpression> filter_expr
	);
	std::string render(IContext* ctx) override;
};

__RENDER_INTERNAL_END__
