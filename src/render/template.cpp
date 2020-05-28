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

// Framework modules.
#include "./internal/lexer.h"
#include "./internal/parser.h"


__RENDER_BEGIN__

Template::Template(const std::string& code, BaseEngine* engine)
{
	this->_engine = engine;
	this->_template_code = code;
	this->compile();
}

void Template::compile()
{
	auto lexer = internal::lexer(this->_template_code);
	lexer.tokenize();

	auto parser = internal::parser(
		lexer.tokens,
		this->_engine->get_filters(),
		this->_engine->get_tags()
	);

	this->_nodes = std::move(parser.parse());
}

std::string Template::render(IContext* ctx)
{
	return this->_nodes->render(ctx);
}

__RENDER_END__
