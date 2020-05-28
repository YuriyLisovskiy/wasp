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
 * An implementation of render/response.h
 */

#include "./response.h"

// Framework modules.
#include "./exceptions.h"


__RENDER_BEGIN__

TemplateResponse::TemplateResponse(
	env::IEnvironment* env,
	const std::string& template_name,
	IContext* context,
	unsigned short int status,
	const std::string& content_type,
	const std::string& charset
) : HttpResponse("", status, content_type, "", charset)
{
	this->_env = env;
	this->_template_name = template_name;
	this->_context = context;
	this->_is_rendered = false;
}

void TemplateResponse::render()
{
	if (this->_is_rendered)
	{
		return;
	}

	auto t = this->_env->get_template(this->_template_name);
	this->_content = t->render(this->_context);
	this->_is_rendered = true;
}

std::string TemplateResponse::get_content()
{
	if (this->_is_rendered)
	{
		return this->_content;
	}

	throw ContentNotRenderedError(
		"The response content must be rendered before it can be accessed.",
		_ERROR_DETAILS_
	);
}

__RENDER_END__
