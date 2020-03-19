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
#include <stack>
#include <algorithm>
#include <memory>

// Module definitions.
#include "../_def_.h"

// Wasp libraries.
#include "./lexer.h"
#include "./token.h"
#include "./node_list.h"
#include "./filter_expr.h"
#include "../library/base.h"
#include "../exceptions.h"
#include "../../core/string/str.h"
#include "../../core/utility.h"


__RENDER_INTERNAL_BEGIN__

struct parser
{
	std::vector<token_t> tokens;
	lib::Filters filters;
	lib::Tags tags;
	std::stack<std::pair<std::string, token_t>> command_stack;
	std::unique_ptr<node_list> nodes_list;

	parser(
		std::vector<token_t>& tokens,
		lib::Filters& filters,
		lib::Tags& tags
	);

	void parse(
		const std::vector<std::string>& parse_until = {}
	);
	void skip_past(const std::string& end_tag);
	token_t next_token();
	void prepend_token(token_t& t);
	void del_first_token();
	FilterExpression compile_filter(token_t& t);
	static void append_node(
		std::unique_ptr<node_list>& list,
		std::shared_ptr<node>& nd,
		const token_t& token
	);

	static void throw_error(
		const std::string& e,
		const token_t& t,
		int line = 0,
		const char* function = "",
		const char* file = ""
	);
	void unclosed_block_tag(
		const std::vector<std::string>& parse_until
	);
	static void invalid_block_tag(
		token_t& token,
		const std::string& command,
		const std::vector<std::string>& parse_until
	);
};

__RENDER_INTERNAL_END__
