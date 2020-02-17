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
#include <vector>

// Module definitions.
#include "../_def_.h"

// Wasp libraries.
#include "./base.h"
#include "./token.h"
#include "../base.h"
#include "../builtins.h"
#include "../exceptions.h"
#include "../../core/object/object.h"
#include "../../collections/dict.h"


__RENDER_INTERNAL_BEGIN__

const std::string NUM = R"([-+\.]?\d[\d\.e]*)";
const std::string VAR_CHARS = R"(\w\.)";

// variable|default(val="Default value")
const std::string FILTER_REGEX(
	"<constant>" + CONST_STRING + "|" +
	"<var>([" + VAR_CHARS + "]+|" + NUM + ")|" +
	R"((?:\s*)" + re::escape(FILTER_SEP) + R"(\s*)" +
	R"(<filter_name>(\w+))" +
	R"((?:\()" + "<arg_name>([" + VAR_CHARS + "]+)" + re::escape(FILTER_ARG_SEP) +
	"(?:<constant_arg>" + CONST_STRING + "|" +
	"<var_arg>([" + VAR_CHARS + "]+|" + NUM + R"())\))?))"
);

/// Parse a variable token and its
/// optional filters (all as a single string).
class FilterExpression
{
private:
	std::vector<std::pair<Filter, collections::Dict<std::string, Variable*>>> _filters;
	Variable* _var = nullptr;
	core::object::Object* _var_obj = nullptr;

public:
	FilterExpression() = default;
	~FilterExpression();

	explicit FilterExpression(token_t& token, Filters& builtins);
	std::string resolve(IContext* ctx);
};

__RENDER_INTERNAL_END__
