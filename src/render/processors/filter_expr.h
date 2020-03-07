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
#include <functional>

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

struct parsed_arg
{
	std::string name;
	std::string value;
	bool is_const;
};

struct parsed_filter
{
	std::string name;
	std::vector<parsed_arg> args;
};

struct parsed_expr
{
	std::string var_name;
	bool is_const;
	std::vector<std::string> var_attrs;
	std::vector<parsed_filter> filters;
};

struct expression_parser
{
	token_t token;
	char ch;
	parsed_expr expression;
	size_t symbol_pos;

	enum p_state
	{
		s_begin,
		s_var,
		s_var_attr_begin,
		s_var_attr,
		s_const_str,
		s_filter_sep,
		s_filter_name_begin,
		s_filter_name,
		s_param_name_begin,
		s_param_name,
		s_param_value_begin,
		s_param_value_var,
		s_param_value_const_str,
		s_param_sep
	} st;

	void parse();

	/// Skips whitespaces.
	/// Returns true if it equals to cend()
	bool skip_ws_and_ret(std::string::const_iterator& it);
	bool parse_number_and_ret(std::string::const_iterator& it, std::string& value);
	void parse_digits(std::string::const_iterator& it, std::string& value);
	void throw_unexpected_symbol(char ch);
	static bool is_var_char(char ch);
	static bool is_var_char_begin(char ch);
};

/// Parse a variable token and its
/// optional filters (all as a single string).
class FilterExpression
{
private:
	token_t _token;
	Variable* _var = nullptr;
	std::vector<std::function<void(
		core::object::Object* obj, IContext* ctx
	)>> _filters;

public:
	FilterExpression() = default;
	~FilterExpression();

	explicit FilterExpression(token_t& token, Filters& builtins);
	std::string resolve(IContext* ctx);
};

__RENDER_INTERNAL_END__
