/**
 * controllers/template.cpp
 *
 * Copyright (c) 2020-2021 Yuriy Lisovskiy
 */

#include "./template.h"

// Framework libraries.
#include "../render/response.h"


__CONTROLLERS_BEGIN__

TemplateResponseMixin::TemplateResponseMixin(render::IEngine* engine)
{
	if (!engine)
	{
		throw ImproperlyConfigured(
			"Template engine must be instantiated in order to use rendering.", _ERROR_DETAILS_
		);
	}

	this->engine = engine;
}

std::unique_ptr<http::HttpResponse> TemplateResponseMixin::render(
	http::Request* request,
	const std::shared_ptr<render::IContext>& context,
	const std::string& template_name,
	unsigned short int status,
	const std::string& content_type,
	const std::string& charset
) const
{
	auto response = std::make_unique<render::TemplateResponse>(
		this->engine,
		template_name.empty() ? this->get_template_name() : template_name,
		context.get(),
		status,
		content_type,
		charset
	);
	response->render();
	return response;
}

std::string TemplateResponseMixin::get_template_name() const
{
	if (this->template_name.empty())
	{
		throw ImproperlyConfigured(
			"TemplateResponseMixin requires either a definition of 'template_name' or an "
			"implementation of 'get_template_name()'",
			_ERROR_DETAILS_
		);
	}

	return this->template_name;
}

__CONTROLLERS_END__
