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
 * An implementation of render/internal/node_list.h
 */

#include "./node_list.h"


__RENDER_INTERNAL_BEGIN__

/// Struct node_list
node_list::node_list()
{
	this->contains_non_text = false;
}

void node_list::append(const std::shared_ptr<node>& node)
{
	this->nodes.push_back(node);
}

std::string node_list::render(IContext* ctx)
{
	std::string result;
	for (const auto& node : this->nodes)
	{
		result += node->render(ctx);
	}

	return result;
}

__RENDER_INTERNAL_END__
