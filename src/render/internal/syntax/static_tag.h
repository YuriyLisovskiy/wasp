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
 * render/library/syntax/static_tag.h
 *
 * Purpose:
 * TODO:
 */

#pragma once

// Module definitions.
#include "./_def_.h"

// Framework modules.
#include "../parser.h"


__SYNTAX_BEGIN__

const std::string TAG_NAME_STATIC = "static";
const std::string TAG_NAME_MEDIA = "media";

struct static_node : public node
{
	std::string prefix;
	std::string var_name;
	std::shared_ptr<FilterExpression> path;

	std::string render(IContext* ctx) override;
};

extern std::function<std::shared_ptr<internal::node>(
	internal::parser*, internal::token_t& token
)> make_static_tag(const std::string& name, const std::string& prefix);

__SYNTAX_END__
