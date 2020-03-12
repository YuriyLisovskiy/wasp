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
 * An implementation of render/template.h
 */

#include "./template.h"


__RENDER_BEGIN__

Template::Template(const std::string& code, BaseEngine* engine)
{
	// TODO: implement Template(const std::string& code, BaseEngine* engine)

	this->_engine = engine;
	this->_template_code = code;
}

void Template::compile()
{
	// TODO: implement void Template::compile()
}

std::string Template::render(const std::shared_ptr<IContext>& context)
{
	// TODO: implement Template::render(BaseContext* context)

	auto lexer = internal::lexer(this->_template_code);
	lexer.tokenize();

	// TODO: tmp
	std::string result;
	for (auto& token : lexer.tokens)
	{
		if (token.type == internal::token_type::var)
		{
			result += internal::FilterExpression(token, DEFAULT_FILTERS).resolve(context);
		}
		else
		{
			result += token.content;
		}
	}

	return result;
}

__RENDER_END__
