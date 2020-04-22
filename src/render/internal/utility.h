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
 * render/internal/utility.h
 *
 * Purpose:
 * TODO:
 */

#pragma once

// C++ libraries.
#include <string>
#include <vector>
#include <memory>

// Module definitions.
#include "../_def_.h"

// Framework modules
#include "./filter_expr.h"


__RENDER_INTERNAL_BEGIN__

extern bool split_params(
	const std::string& params_str,
	size_t line_no,
	size_t& curr_pos,
	std::vector<token_t>& params
);

extern bool is_var_char(char ch);

extern bool is_var_char_begin(char ch);

extern bool trim_quotes(std::string& str);

extern bool split_for_loop_vars(
	const std::string& vars_str,
	size_t line_no,
	size_t& curr_pos,
	std::vector<token_t>& vars,
	size_t max_vars
);

__RENDER_INTERNAL_END__
