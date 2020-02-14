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
 * render/processors/parser.h
 *
 * Purpose:
 * TODO: write docs for render/processors/parser.h
 */

#pragma once

// C++ libraries.
#include <vector>
#include <algorithm>

// Module definitions.
#include "../_def_.h"

// Wasp libraries.
#include "./lexer.h"
#include "../builtins.h"
#include "../exceptions.h"


__RENDER_INTERNAL_BEGIN__

struct parser
{
	std::vector<token> tokens;
	Builtins builtins;

	parser(std::vector<token>& tokens, Builtins& builtins);

	void skip_past(const std::string& end_tag);
	void throw_error(
		token& t,
		std::string& e,
		int line,
		const char* function,
		const char* file
	);
	token next_token();
	void prepend_token(token& t);
	void del_first_token();
	void compile_filter(token& t);
};

__RENDER_INTERNAL_END__
