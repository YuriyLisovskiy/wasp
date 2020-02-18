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

void expression_parser::parse()
{
	std::string last_filter_name, last_param_name, last_param_value, last_var_attr;

	std::vector<parsed_arg> args;

	char const_str_quote = '"';

	p_state st = p_state::s_begin;
	auto it = this->token.content.cbegin();
	while (it != this->token.content.cend())
	{
		char ch = *it++;
		switch (st)
		{
			case p_state::s_begin:
				if (ch == '\'' || ch == '"')
				{
					st = p_state::s_const_str;
					const_str_quote = ch;
					this->expression.var_name += ch;
				}
				else if (std::isalpha(ch) || ch == '_')
				{
					st = p_state::s_var;
					this->expression.var_name += ch;
				}
				else
				{
					this->throw_unexpected_symbol(ch);
				}
				break;
			case p_state::s_var:
				if (is_var_char(ch))
				{
					this->expression.var_name += ch;
				}
				else
				{
					if (ch == VAR_ATTR_SEP)
					{
						st = p_state::s_var_attr_begin;
					}
					// TODO: skip whitespaces first
					else if (ch == FILTER_SEP)
					{
						st = p_state::s_filter_name_begin;
					}
					else
					{
						this->throw_unexpected_symbol(ch);
					}
				}
				break;
			case p_state::s_var_attr_begin:
				// TODO: skip whitespaces first
				if (is_var_char_begin(ch))
				{
					last_var_attr.clear();
					last_var_attr += ch;
					st = p_state::s_var_attr;
				}
				else
				{
					this->throw_unexpected_symbol(ch);
				}
				break;
			case p_state::s_var_attr:
				if (is_var_char(ch))
				{
					last_var_attr += ch;
				}
				else
				{
					// TODO: skip whitespaces first
					if (ch == '.')
					{
						st = p_state::s_var_attr_begin;
					}
					else if (ch == FILTER_SEP)
					{
						st = p_state::s_filter_name_begin;
					}
					else
					{
						this->throw_unexpected_symbol(ch);
					}

					this->expression.var_attrs.push_back(last_var_attr);
				}
				break;
			case p_state::s_const_str:
				if (ch == const_str_quote)
				{
					st = p_state::s_filter_sep;
				}

				this->expression.var_name += ch;
				break;
			case p_state::s_filter_sep:
				// TODO: skip whitespaces first
				if (ch == FILTER_SEP)
				{
					args.clear();
					st = p_state::s_filter_name_begin;
				}
				else
				{
					this->throw_unexpected_symbol(ch);
				}
				break;
			case p_state::s_filter_name_begin:
				// TODO: skip whitespaces first
				if (is_var_char_begin(ch))
				{
					last_filter_name.clear();
					st = p_state::s_filter_name;
				}
				else
				{
					this->throw_unexpected_symbol(ch);
				}

				last_filter_name += ch;
				break;
			case p_state::s_filter_name:
				if (is_var_char(ch))
				{
					last_filter_name += ch;
				}
				else
				{
					// TODO: skip whitespaces first
					if (ch == '(')
					{
						st = p_state::s_param_name_begin;
					}
					else
					{
						this->throw_unexpected_symbol(ch);
					}
				}
				break;
			case p_state::s_param_name_begin:
				// TODO: skip whitespaces first
				if (is_var_char_begin(ch))
				{
					last_param_name.clear();
					last_param_value.clear();
					last_param_name += ch;
					st = p_state::s_param_name;
				}
				else
				{
					this->throw_unexpected_symbol(ch);
				}
				break;
			case p_state::s_param_name:
				if (is_var_char(ch))
				{
					last_param_name += ch;
				}
				else
				{
					// TODO: skip whitespaces first
					if (ch == FILTER_ARG_NAME_VAL_SEP)
					{
						st = p_state::s_param_value_begin;
					}
					else
					{
						this->throw_unexpected_symbol(ch);
					}
				}
				break;
			case p_state::s_param_value_begin:
				// TODO: skip whitespaces first
				if (ch == '\'' || ch == '"')
				{
					st = p_state::s_param_value_const_str;
					const_str_quote = ch;
					last_param_value += ch;
				}
				else if (std::isalpha(ch) || ch == '_')
				{
					st = p_state::s_param_value_var;
					last_param_value += ch;
				}
				else
				{
					this->throw_unexpected_symbol(ch);
				}
				break;
			case p_state::s_param_value_var:
				if (is_var_char(ch))
				{
					last_param_value += ch;
				}
				else
				{
					// TODO: skip whitespaces first
					if (ch == FILTER_ARG_SEP)
					{
						st = p_state::s_param_name_begin;
						args.push_back({last_param_name, last_param_value});
					}
					else if (ch == ')')
					{
						st = p_state::s_filter_sep;
						args.push_back({last_param_name, last_param_value});
						this->expression.filters.push_back({last_filter_name, args});
					}
					else
					{
						this->throw_unexpected_symbol(ch);
					}
				}
				break;
			case p_state::s_param_value_const_str:
				if (ch == const_str_quote)
				{
					st = p_state::s_param_sep;
				}

				last_param_value += ch;
				break;
			case p_state::s_param_sep:
				// TODO: skip whitespaces first
				if (ch == FILTER_ARG_SEP)
				{
					st = p_state::s_param_name_begin;
					args.push_back({last_param_name, last_param_value});
				}
				else if (ch == ')')
				{
					st = p_state::s_filter_sep;
					args.push_back({last_param_name, last_param_value});
					this->expression.filters.push_back({last_filter_name, args});
				}
				else
				{
					this->throw_unexpected_symbol(ch);
				}
				break;
		}
	}
}

bool expression_parser::is_var_char(char ch)
{
	return std::isalnum(ch) || ch == '_';
}

bool expression_parser::is_var_char_begin(char ch)
{
	return std::isalpha(ch) || ch == '_';
}

void expression_parser::throw_unexpected_symbol(char ch)
{
	throw TemplateSyntaxError(
		"Unexpected symbol '" + std::string(ch, 1) +
		"' in statement: '" + this->token.content + "'",
		this->token
	);
}


FilterExpression::FilterExpression(token_t& token, Filters& builtins)
{
	this->_token = std::move(token);
	expression_parser p{std::move(this->_token)};
	p.parse();

	this->_var = new Variable(p.expression.var_name, p.expression.var_attrs);
	for (const auto& filter : p.expression.filters)
	{
		std::vector<std::pair<std::string, Variable*>> args;
		for (const auto& arg : filter.args)
		{
			args.emplace_back(arg.name, new Variable(arg.value, {}));
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
