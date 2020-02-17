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
#include <regex>

// Module definitions.
#include "../_def_.h"

// Wasp libraries.
#include "./base.h"
#include "./token.h"
#include "../base.h"
#include "../builtins.h"


__RENDER_INTERNAL_BEGIN__

const std::string NUM = R"([-+\.]?\d[\d\.e]*)";
const std::string VAR_CHARS = R"(\w\.)";

const std::string FILTER_REGEX(
	"<constant>" + CONST_STRING + "|" +
	"<var>([" + VAR_CHARS + "]+|" + NUM + ")|" +
	R"((?:\s*)" + re::escape(FILTER_SEP) + R"(\s*)" +
	R"(<filter_name>(\w+))" +
	"(?:" + re::escape(FILTER_ARG_SEP) +
	"(?:<constant_arg>" + CONST_STRING + "|" +
	"<var_arg>([" + VAR_CHARS + "]+|" + NUM + ")))?)"
);

/// Parse a variable token and its optional filters (all as a single string),
/// and return a list of tuples of the filter name and arguments.
class FilterExpression
{
public:
	FilterExpression() = default;

	explicit FilterExpression(token_t& token, Builtins& filters)
	{
		core::rgx::ArgRegex filter_regex(FILTER_REGEX);
		// TODO: implement FilterExpression(token_t& token, Builtins& filters)
	}

	std::string resolve(IContext* ctx)
	{
		return "";
	}
};

__RENDER_INTERNAL_END__
