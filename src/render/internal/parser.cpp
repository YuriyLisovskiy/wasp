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

parser::parser(std::vector<token_t>& tokens, lib::Filters& filters, lib::Tags& tags)
{
	std::reverse(tokens.begin(), tokens.end());
	this->tokens = tokens;
	this->filters = filters;
	this->tags = tags;
}

std::shared_ptr<node_list> parser::parse(
	const std::vector<std::string>& parse_until
)
{
	auto nodes_list = std::make_shared<node_list>();
	while (!this->tokens.empty())
	{
		token_t token = this->next_token();
		if (token.type == token_type::text)
		{
			auto t_node = std::shared_ptr<node>(new text_node(token.content));
			parser::append_node(nodes_list, t_node, token);
		}
		else if (token.type == token_type::var)
		{
			if (token.content.empty())
			{
				parser::throw_error(
					"Empty variable tag on line " + std::to_string(token.line_no), token
				);
			}

			try
			{
				auto filer_expr = this->compile_filter(token);
				auto var_node = std::shared_ptr<node>(new variable_node(filer_expr));
				parser::append_node(nodes_list, var_node, token);
			}
			catch (const core::BaseException& exc)
			{
				parser::throw_error(exc.what(), token);
			}
		}
		else if (token.type == token_type::block)
		{
			if (token.content.empty())
			{
				parser::throw_error(
					"Empty block tag on line " + std::to_string(token.line_no), token
				);
			}

			auto command = get_command(token.content);
			if (command.empty())
			{
				parser::throw_error(
					"Empty command on line " + std::to_string(token.line_no), token
				);
			}

			if (core::utility::contains(command, parse_until))
			{
				// A matching token has been reached. Return control to
				// the caller. Put the token back on the token list so the
				// caller knows where it terminated.
				this->prepend_token(token);
				return nodes_list;
			}

			// Add the token to the command stack. This is used for error
			// messages if further parsing fails due to an unclosed block
			// tag.
			this->command_stack.push({command, token});

			// Get the tag callback function from the ones registered with
			// the parser.
			if (!this->tags.contains(command))
			{
				parser::invalid_block_tag(token, command, parse_until);
			}

			auto compile_func = this->tags.get(command);
			try
			{
				// Compile the callback into a node and add it to
				// the node list.
				auto compiled_node = compile_func(this, token);
				parser::append_node(nodes_list, compiled_node, token);

				// Compile success. Remove the token from the command stack.
				this->command_stack.pop();
			}
			catch (const core::BaseException& exc)
			{
				parser::throw_error(exc.what(), token);
			}
		}
	}

	if (!parse_until.empty())
	{
		this->unclosed_block_tag(parse_until);
	}

	return nodes_list;
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

std::shared_ptr<FilterExpression> parser::compile_filter(token_t& t)
{
	return std::make_shared<FilterExpression>(t, this->filters);
}

void parser::append_node(
	std::shared_ptr<node_list>& list,
	std::shared_ptr<node>& nd,
	const token_t& token
)
{
	if (!nd)
	{
		return;
	}

	// Check that non-text nodes don't appear before an extends tag.
	if (nd->must_be_first && list->contains_non_text)
	{
		parser::throw_error(
			nd->token.content + " must be the first tag in the template.",
			token
		);
	}

	if (!nd->is_text_node)
	{
		list->contains_non_text = true;
	}

	nd->token = token;
	list->append(nd);
}

void parser::throw_error(
	const std::string& e,
	const token_t& t,
	int line,
	const char* function,
	const char* file
)
{
	throw TemplateSyntaxError(e, t, line, function, file);
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

void parser::invalid_block_tag(
	token_t& token,
	const std::string& command,
	const std::vector<std::string>& parse_until
)
{
	if (!parse_until.empty())
	{
		parser::throw_error(
			"Invalid block tag on line " + std::to_string(token.line_no) +
			": '" + command + "', expected " +
			core::str::make_text_list(parse_until, "or") +
			". Did you forget to register this tag?",
			token
		);
	}

	parser::throw_error(
		"Invalid block tag on line " + std::to_string(token.line_no) +
		": '" + command + "'. Did you forget to register this tag?",
		token
	);
}

std::string parser::get_command(const std::string& content)
{
	std::string command;
	if (!content.empty())
	{
		auto it = content.begin();
		char ch = *it++;
		if (std::isalpha(ch) || ch == '_')
		{
			command += ch;
			while (it != content.end())
			{
				ch = *it++;
				if (std::isalnum(ch) || ch == '_')
				{
					command += ch;
				}
				else
				{
					break;
				}
			}
		}
	}

	return command;
}

void parser::invalid_syntax(token_t& token, size_t pos)
{
	parser::throw_error(
		"Invalid syntax on line " + std::to_string(token.line_no) +
		", position " + std::to_string(pos) + ": '" + token.content + "'",
		token
	);
}

__RENDER_INTERNAL_END__
