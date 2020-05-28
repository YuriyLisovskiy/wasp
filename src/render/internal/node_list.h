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
#include "./nodes.h"


__RENDER_INTERNAL_BEGIN__

struct node_list
{
	bool contains_non_text;
	std::vector<std::shared_ptr<node>> nodes;

	node_list();
	void append(const std::shared_ptr<node>& node);
	std::string render(IContext* ctx);
};

__RENDER_INTERNAL_END__
