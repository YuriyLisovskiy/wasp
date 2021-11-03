/**
 * render/response.cpp
 *
 * Copyright (c) 2020-2021 Yuriy Lisovskiy
 */

#include "./response.h"

// Base libraries.
#include <xalwart.base/exceptions.h>


__RENDER_BEGIN__

TemplateResponse::TemplateResponse(
	render::IEngine* engine,
	const std::string& template_name,
	render::IContext* context,
	unsigned short int status,
	const std::string& content_type,
	const std::string& charset
) : Response(status, "", content_type, "", charset)
{
	this->_engine = engine;
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

	auto template_ = require_non_null(
		this->_engine, "'engine' is nullptr", _ERROR_DETAILS_
	)->get_template(this->_template_name);
	this->set_content(require_non_null(
		template_.get(), "template is nullptr", _ERROR_DETAILS_
	)->render(this->_context));
	this->_is_rendered = true;
}

std::string TemplateResponse::get_content() const
{
	if (this->_is_rendered)
	{
		return http::Response::get_content();
	}

	throw RuntimeError(
		"The response content must be rendered before it can be accessed.", _ERROR_DETAILS_
	);
}

__RENDER_END__
