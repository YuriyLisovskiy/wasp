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
 * An implementation of render/processors/parser.h
 */

#include "./parser.h"


__RENDER_INTERNAL_BEGIN__

parser::parser(std::vector<token>& tokens, Builtins& builtins)
{
	std::reverse(tokens.begin(), tokens.end());
	this->tokens = std::move(tokens);
	this->builtins = std::move(builtins);
}

void parser::skip_past(const std::string& end_tag)
{
	// TODO: implement skip_past(const std::string& end_tag)
}

void parser::throw_error(
	token& t,
	std::string& e,
	int line,
	const char* function,
	const char* file
)
{
	throw TemplateSyntaxError(e, line, function, file);
}

token parser::next_token()
{
	token res = this->tokens.back();
	this->tokens.pop_back();
	return res;
}

void parser::prepend_token(token& t)
{
	this->tokens.push_back(t);
}

void parser::del_first_token()
{
	this->tokens.pop_back();
}

void parser::compile_filter(token& t)
{
	// TODO: implement compile_filter(token& t)
}

__RENDER_INTERNAL_END__
