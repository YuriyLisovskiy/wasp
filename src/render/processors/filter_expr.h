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
 * render/processors/filter_expr.h
 *
 * Purpose:
 * TODO:
 */

#pragma once

// C++ libraries.
#include <string>

// Module definitions.
#include "../_def_.h"

// Wasp libraries.
#include "./token.h"
#include "../base.h"
#include "../builtins.h"


__RENDER_INTERNAL_BEGIN__

// TODO: implement class FilterExpression
class FilterExpression
{
public:
	FilterExpression() = default;

	explicit FilterExpression(token_t& token, Builtins& filters)
	{

	}

	std::string resolve(IContext* ctx)
	{
		return "";
	}
};

__RENDER_INTERNAL_END__
