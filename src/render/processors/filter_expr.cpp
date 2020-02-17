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

FilterExpression::~FilterExpression()
{
	delete this->_var;
	delete this->_var_obj;
}

FilterExpression::FilterExpression(token_t& token, Filters& builtins)
{
	core::rgx::ArgRegex filter_regex(FILTER_REGEX);
	size_t up_to = 0;
	filter_regex.search_iter(token.content);
	this->_var = nullptr;
	while (filter_regex.next())
	{
		if (!this->_var_obj && !this->_var)
		{
			auto var = filter_regex.group("var");
			auto constant = filter_regex.group("constant");
			if (!constant.empty())
			{
				this->_var_obj = Variable(constant).resolve(nullptr);
			}
			else if (var.empty())
			{
				throw TemplateSyntaxError(
					"Could not find variable at start of " + token.content + ".",
					token
				);
			}
			else
			{
				this->_var = new Variable(var);
			}
		}
		else
		{
			auto filter_name = filter_regex.group("filter_name");

		}
	}
}

std::string FilterExpression::resolve(IContext* ctx)
{
	return "";
}

__RENDER_INTERNAL_END__
