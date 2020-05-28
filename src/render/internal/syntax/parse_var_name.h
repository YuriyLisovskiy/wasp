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
 * render/internal/syntax/parse_var_name.h
 *
 * Purpose:
 * 	Parses variable assigning, i.e. if we have 'tag_name(param_1, ...) -> var_name' code,
 * 	function will parse '-> var_name' and retrieve variable name.
 */

#pragma once

// Module definitions.
#include "./_def_.h"

// Framework modules.
#include "../token.h"


__SYNTAX_BEGIN__

bool parse_var_name(
	internal::token_t& token,
	std::string& var_name,
	size_t& curr_pos,
	bool has_content_after_var
);

__SYNTAX_END__
