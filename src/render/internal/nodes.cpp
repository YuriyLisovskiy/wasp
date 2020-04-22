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
 * An implementation of render/internal/nodes.h
 */

#include "./nodes.h"


__RENDER_INTERNAL_BEGIN__

/// Struct node
node::node()
{
	this->must_be_first = false;
	this->is_text_node = false;
}

std::string node::render(IContext* ctx)
{
	return "";
}

/// Struct text_node
text_node::text_node() : node()
{
	this->is_text_node = true;
}

text_node::text_node(const std::string& s) : text_node()
{
	this->text = s;
}

std::string text_node::render(IContext* ctx)
{
	return this->text;
}

/// Struct variable_node
variable_node::variable_node(
	std::shared_ptr<FilterExpression> filter_expr
) : node()
{
	this->filter_expr = std::move(filter_expr);
}

std::string variable_node::render(IContext* ctx)
{
	auto expr = this->filter_expr->resolve(ctx);
	return expr ? expr->__str__() : "";
}

__RENDER_INTERNAL_END__
