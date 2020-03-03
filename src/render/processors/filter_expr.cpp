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
 * An implementation of render/processors/filter_expr.h
 */

#include "./filter_expr.h"


__RENDER_INTERNAL_BEGIN__

// TODO: add non-string values parsing
void expression_parser::parse()
{
	std::string last_filter_name, last_param_name, last_param_value, last_var_attr;

	std::vector<parsed_arg> args;

	char const_str_quote = '"';

	this->symbol_pos = 0;
	bool param_is_constant = false;
	this->st = p_state::s_begin;
	auto it = this->token.content.cbegin();
	while (it != this->token.content.cend())
	{
		this->ch = *it++;
		this->symbol_pos++;
		if (this->skip_ws_and_ret(it))
		{
			break;
		}

		switch (this->st)
		{
			case p_state::s_begin:
				if (this->ch == '\'' || this->ch == '"')
				{
					this->st = p_state::s_const_str;
					const_str_quote = this->ch;
					this->expression.var_name += this->ch;
					this->expression.is_const = true;
				}
				if (std::isdigit(this->ch) || std::isdigit(this->ch) || this->ch == '+' || this->ch == '-')
				{
					this->expression.var_name += this->ch;
					this->expression.is_const = true;
					if (this->parse_number_and_ret(it, this->expression.var_name))
					{
						return;
					}

					this->st = p_state::s_filter_sep;

			//		this->st = p_state::s_const_number_before_dot;
			//		this->expression.var_name += this->ch;
			//		this->expression.is_const = true;
				}
				else if (std::isalpha(this->ch) || this->ch == '_')
				{
					this->st = p_state::s_var;
					this->expression.var_name += this->ch;
					this->expression.is_const = false;
				}
				else
				{
					this->throw_unexpected_symbol(this->ch);
				}
				break;
			case p_state::s_var:
				if (is_var_char(this->ch))
				{
					this->expression.var_name += this->ch;
				}
				else
				{
					if (this->ch == VAR_ATTR_SEP)
					{
						this->st = p_state::s_var_attr_begin;
					}
					else if (this->ch == FILTER_SEP)
					{
						this->st = p_state::s_filter_name_begin;
					}
					else
					{
						this->throw_unexpected_symbol(this->ch);
					}
				}
				break;
			case p_state::s_var_attr_begin:
				if (is_var_char_begin(this->ch))
				{
					last_var_attr.clear();
					last_var_attr += this->ch;
					this->st = p_state::s_var_attr;
				}
				else
				{
					this->throw_unexpected_symbol(this->ch);
				}
				break;
			case p_state::s_var_attr:
				if (is_var_char(this->ch))
				{
					last_var_attr += this->ch;
				}
				else
				{
					if (this->ch == '.')
					{
						this->st = p_state::s_var_attr_begin;
					}
					else if (this->ch == FILTER_SEP)
					{
						this->st = p_state::s_filter_name_begin;
					}
					else
					{
						this->throw_unexpected_symbol(this->ch);
					}

					this->expression.var_attrs.push_back(last_var_attr);
				}
				break;
			case p_state::s_const_str:
				if (this->ch == const_str_quote)
				{
					this->st = p_state::s_filter_sep;
				}

				this->expression.var_name += this->ch;
				break;
			/*
			case p_state::s_const_number_before_dot:
				if (this->ch == '.')
				{
					this->st = p_state::s_const_number_after_dot;
					this->expression.var_name += this->ch;
				}
				else if (std::isdigit(this->ch))
				{
					this->expression.var_name += this->ch;
				}
				else if (this->ch == ' ' || this->ch == FILTER_SEP)
				{
					it--;
					this->st = p_state::s_filter_sep;
				}
				else
				{
					this->throw_unexpected_symbol(this->ch);
				}
				break;
			case p_state::s_const_number_after_dot:
				if (std::isdigit(this->ch))
				{
					this->expression.var_name += this->ch;
				}
				else if (this->ch == ' ' || this->ch == FILTER_SEP)
				{
					it--;
					this->st = p_state::s_filter_sep;
				}
				else
				{
					this->throw_unexpected_symbol(this->ch);
				}
				break;
			*/
			case p_state::s_filter_sep:
				if (this->ch == FILTER_SEP)
				{
					args.clear();
					this->st = p_state::s_filter_name_begin;
				}
				else
				{
					this->throw_unexpected_symbol(this->ch);
				}
				break;
			case p_state::s_filter_name_begin:
				if (is_var_char_begin(this->ch))
				{
					last_filter_name = this->ch;
					this->st = p_state::s_filter_name;
				}
				else
				{
					this->throw_unexpected_symbol(this->ch);
				}
				break;
			case p_state::s_filter_name:
				if (is_var_char(this->ch))
				{
					last_filter_name += this->ch;
				}
				else
				{
					if (this->ch == '(')
					{
						this->st = p_state::s_param_name_begin;
					}
					else
					{
						this->throw_unexpected_symbol(this->ch);
					}
				}
				break;
			case p_state::s_param_name_begin:
				if (is_var_char_begin(this->ch))
				{
					last_param_name.clear();
					last_param_value.clear();
					last_param_name += this->ch;
					this->st = p_state::s_param_name;
				}
				else
				{
					this->throw_unexpected_symbol(ch);
				}
				break;
			case p_state::s_param_name:
				if (is_var_char(this->ch))
				{
					last_param_name += this->ch;
				}
				else
				{
					if (this->ch == FILTER_ARG_NAME_VAL_SEP)
					{
						this->st = p_state::s_param_value_begin;
					}
					else
					{
						this->throw_unexpected_symbol(this->ch);
					}
				}
				break;
			case p_state::s_param_value_begin:
				if (this->ch == '\'' || this->ch == '"')
				{
					this->st = p_state::s_param_value_const_str;
					const_str_quote = this->ch;
					last_param_value += this->ch;
				}
				else if (std::isdigit(this->ch) || this->ch == '+' || this->ch == '-')
				{
					last_param_value += this->ch;
					if (this->parse_number_and_ret(it, last_param_value))
					{
						this->throw_unexpected_symbol(this->ch);
					}

					param_is_constant = true;
					this->st = p_state::s_param_sep;
				}
				else if (std::isalpha(this->ch) || this->ch == '_')
				{
					this->st = p_state::s_param_value_var;
					param_is_constant = false;
					last_param_value += this->ch;
				}
				else
				{
					this->throw_unexpected_symbol(this->ch);
				}
				break;
			case p_state::s_param_value_var:
				if (is_var_char(this->ch))
				{
					last_param_value += this->ch;
				}
				else
				{
					if (this->ch == FILTER_ARG_SEP)
					{
						this->st = p_state::s_param_name_begin;
						args.push_back({last_param_name, last_param_value, param_is_constant});
					}
					else if (this->ch == ')')
					{
						this->st = p_state::s_filter_sep;
						args.push_back({last_param_name, last_param_value, param_is_constant});
						this->expression.filters.push_back({last_filter_name, args});
					}
					else
					{
						this->throw_unexpected_symbol(this->ch);
					}
				}
				break;
			case p_state::s_param_value_const_str:
				if (this->ch == const_str_quote)
				{
					this->st = p_state::s_param_sep;
					param_is_constant = true;
				}

				last_param_value += this->ch;
				break;
			case p_state::s_param_sep:
				if (this->ch == FILTER_ARG_SEP)
				{
					this->st = p_state::s_param_name_begin;
					args.push_back({last_param_name, last_param_value, param_is_constant});
				}
				else if (this->ch == ')')
				{
					this->st = p_state::s_filter_sep;
					args.push_back({last_param_name, last_param_value, param_is_constant});
					this->expression.filters.push_back({last_filter_name, args});
				}
				else
				{
					this->throw_unexpected_symbol(this->ch);
				}
				break;
		}
	}
}

bool expression_parser::parse_number_and_ret(
	std::string::const_iterator& it, std::string& value
)
{
	auto end = this->token.content.cend();
	if (!std::isdigit(*it))
	{
		if (value[0] == '+' || value[0] == '-')
		{
			this->throw_unexpected_symbol(value[0]);
		}
	}

	this->parse_digits(it, value);
	bool is_double = *it == '.';
	if (is_double)
	{
		value += *it++;
		if (!std::isdigit(*it))
		{
			this->throw_unexpected_symbol(*it);
		}

		this->parse_digits(it, value);
	}

	return it == end;
}

void expression_parser::parse_digits(std::string::const_iterator& it, std::string& value)
{
	do
	{
		value += *it++;
	}
	while (std::isdigit(*it) && it != this->token.content.cend());
}

bool expression_parser::skip_ws_and_ret(std::string::const_iterator& it)
{
	if (this->ch != ' ')
	{
		return false;
	}

	if (this->st == p_state::s_const_str || this->st == p_state::s_param_value_const_str)
	{
		return false;
	}

	auto end = this->token.content.cend();
	while (*it == ' ' && it != end)
	{
		this->ch = *it++;
		this->symbol_pos++;
	}

	bool is_end = it == end;
	if (!is_end)
	{
		this->ch = *it++;
		this->symbol_pos++;
	}

	return is_end;
}

bool expression_parser::is_var_char(char c)
{
	return std::isalnum(c) || c == '_';
}

bool expression_parser::is_var_char_begin(char c)
{
	return std::isalpha(c) || c == '_';
}

void expression_parser::throw_unexpected_symbol(char c)
{
	throw TemplateSyntaxError(
		"Unexpected symbol '" + std::string(1, c) +
		"' in statement, position " + std::to_string(this->symbol_pos) +
		": '" + this->token.content + "'",
		this->token
	);
}


FilterExpression::FilterExpression(token_t& token, Filters& builtins)
{
	this->_token = std::move(token);
	expression_parser p{std::move(this->_token)};
	p.parse();

	this->_var = new Variable(p.expression.var_name, p.expression.var_attrs, p.expression.is_const);
	for (const auto& filter : p.expression.filters)
	{
		std::vector<std::pair<std::string, Variable*>> args;
		for (const auto& arg : filter.args)
		{
			args.emplace_back(arg.name, new Variable(arg.value, {}, arg.is_const));
		}

		if (!builtins.contains(filter.name))
		{
			throw FilterDoesNotExist("Filter with name '" + filter.name + "' does not exist");
		}

		this->_filters.emplace_back(
			[args, builtins, filter](core::object::Object* obj, IContext* ctx) mutable {
				std::map<std::string, core::object::Object*> params;
				for (const auto& arg : args)
				{
					params[arg.first] = arg.second->resolve(ctx);
					delete arg.second;
				}

				core::object::Object* new_obj = builtins.get(filter.name)(
					obj, collections::Dict(params, false)
				);
				if (new_obj)
				{
					delete obj;
					obj = new_obj;
				}

				for (const auto& param : params)
				{
					delete param.second;
				}
			}
		);
	}
}

FilterExpression::~FilterExpression()
{
	delete this->_var;
}

std::string FilterExpression::resolve(IContext* ctx)
{
	core::object::Object* var = nullptr;
	try
	{
		var = this->_var->resolve(ctx);
	}
	catch (const VariableDoesNotExist& exc)
	{
		return "";
	}

	if (var)
	{
		for (const auto& filter : this->_filters)
		{
			filter(var, ctx);
		}

		if (var)
		{
			std::string result = var->__str__();
			delete var;
			return result;
		}
	}

	return "";
}

__RENDER_INTERNAL_END__
