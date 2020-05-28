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
 * render/library/base.h
 *
 * Purpose:
 * TODO:
 */

#pragma once

// Module definitions.
#include "./_def_.h"

// Framework modules.
#include "../internal/node_list.h"


__RENDER_INTERNAL_BEGIN__
struct parser;
__RENDER_INTERNAL_END__


__LIB_BEGIN__

typedef collections::Dict<std::string, std::shared_ptr<core::object::Object>> KwArgs;

typedef std::function<std::shared_ptr<core::object::Object>(
	const std::shared_ptr<core::object::Object>&, const KwArgs&
)> Filter;

typedef std::function<std::shared_ptr<internal::node>(
	internal::parser*, internal::token_t& token
)> Tag;

typedef collections::Dict<std::string, Filter> Filters;
typedef collections::Dict<std::string, Tag> Tags;

class ILibrary
{
public:
	virtual ~ILibrary() = default;
	virtual std::shared_ptr<Filters> get_filters() = 0;
	virtual std::shared_ptr<Tags> get_tags() = 0;
	virtual std::string name() = 0;
};

__LIB_END__
