/**
 * render/response.cpp
 *
 * Copyright (c) 2020 Yuriy Lisovskiy
 */

#include "./response.h"

// Render libraries.
#include <xalwart.render/exceptions.h>


__RENDER_BEGIN__

TemplateResponse::TemplateResponse(
	render::IEngine* engine,
	const std::string& template_name,
	IContext* context,
	unsigned short int status,
	const std::string& content_type,
	const std::string& charset
) : HttpResponse("", status, content_type, "", charset)
{
	this->engine = engine;
	this->template_name = template_name;
	this->context = context;
	this->is_rendered = false;
}

void TemplateResponse::render()
{
	if (this->is_rendered)
	{
		return;
	}

	auto t = this->engine->get_template(this->template_name);
	this->_content = t->render(this->context);
	this->is_rendered = true;
}

std::string TemplateResponse::get_content()
{
	if (this->is_rendered)
	{
		return this->_content;
	}

	throw ContentNotRenderedError(
		"The response content must be rendered before it can be accessed.",
		_ERROR_DETAILS_
	);
}

__RENDER_END__
