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

parser::parser(std::vector<token_t>& tokens, Builtins& builtins)
{
	std::reverse(tokens.begin(), tokens.end());
	this->tokens = std::move(tokens);
	this->builtins = std::move(builtins);
}

node_list* parser::parse(
	const std::vector<std::string>& parse_until
)
{
	// TODO: implement parse(const std::vector<std::string>& parse_until)
	return nullptr;
}

void parser::skip_past(const std::string& end_tag)
{
	while (!this->tokens.empty())
	{
		auto token = this->next_token();
		if (token.type == token_type::block && token.content == end_tag)
		{
			return;
		}
	}

	parser::unclosed_block_tag({ end_tag });
}

void parser::throw_error(
	std::string& e,
	token_t& t,
	int line,
	const char* function,
	const char* file
)
{
	throw TemplateSyntaxError(e, t, line, function, file);
}

token_t parser::next_token()
{
	token_t res = this->tokens.back();
	this->tokens.pop_back();
	return res;
}

void parser::prepend_token(token_t& t)
{
	this->tokens.push_back(t);
}

void parser::del_first_token()
{
	this->tokens.pop_back();
}

FilterExpression parser::compile_filter(token_t& t)
{
	return FilterExpression(t, this->builtins);
}

void parser::unclosed_block_tag(
	const std::vector<std::string>& parse_until
)
{
	auto command_token = this->command_stack.top();
	this->command_stack.pop();
	auto message = "Unclosed tag on line " +
		std::to_string(command_token.second.line_no) +
		": '" + command_token.first +
		"'. Looking for one of: " +
		core::str::join(parse_until.begin(), parse_until.end(), ", ") + ".";
	parser::throw_error(message, command_token.second, _ERROR_DETAILS_);
}

__RENDER_INTERNAL_END__
