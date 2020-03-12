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
 * An implementation of render/processors/node_list.h
 */

#include "./node_list.h"


__RENDER_INTERNAL_BEGIN__

/// Struct node
node::node()
{
	this->must_be_first = false;
}

std::string node::render(const std::shared_ptr<IContext>& ctx)
{
	return "";
}

/// Struct text_node
text_node::text_node() : node()
{
}

text_node::text_node(const std::string& s) : node()
{
	this->text = s;
}

std::string text_node::render(const std::shared_ptr<IContext>& ctx)
{
	return this->text;
}

/// Struct variable_node
variable_node::variable_node(
	const FilterExpression& filter_expr
) : node()
{
	this->filter_expr = filter_expr;
}

std::string variable_node::render(const std::shared_ptr<IContext>& ctx)
{
	return this->filter_expr.resolve(ctx);
}

/// Struct node_list
node_list::node_list()
{
	this->contains_non_text = false;
	this->auto_clean = true;
}

node_list::~node_list()
{
	for (auto node : this->nodes)
	{
		delete node;
	}
}

void node_list::append(node* node)
{
	this->nodes.push_back(node);
}

std::string node_list::render(const std::shared_ptr<IContext>& ctx)
{
	std::string result;
	for (auto node : this->nodes)
	{
		result += node->render(ctx);
	}

	return result;
}

__RENDER_INTERNAL_END__
