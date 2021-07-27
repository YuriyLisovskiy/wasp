/**
 * controllers/template.cpp
 *
 * Copyright (c) 2020-2021 Yuriy Lisovskiy
 */

#include "./template.h"

// Framework libraries.
#include "../render/response.h"
#include "../conf/settings.h"


__CONTROLLERS_BEGIN__

TemplateResponseMixin::TemplateResponseMixin(render::abc::IEngine* engine) : template_name(""), content_type("")
{
	if (!engine)
	{
		throw ImproperlyConfigured(
			"Template engine must be initialized in order to use the application",
			_ERROR_DETAILS_
		);
	}

	this->engine = engine;
}

Result<std::shared_ptr<http::IHttpResponse>> TemplateResponseMixin::render(
	http::HttpRequest* request,
	const std::shared_ptr<render::abc::IContext>& context,
	const std::string& template_name,
	unsigned short int status,
	const std::string& content_type,
	const std::string& charset
)
{
	auto response = std::make_shared<render::TemplateResponse>(
		this->engine,
		template_name.empty() ? this->get_template_name() : template_name,
		context.get(),
		status,
		content_type,
		charset
	);
	response->render();
	return Result<std::shared_ptr<http::IHttpResponse>>(response);
}

std::string TemplateResponseMixin::get_template_name()
{
	if (this->template_name.empty())
	{
		throw ImproperlyConfigured(
			"TemplateResponseMixin requires either a definition of '_template_name' or an "
			"implementation of 'get_template_name()'",
			_ERROR_DETAILS_
		);
	}

	return this->template_name;
}


TemplateController::TemplateController(
	conf::Settings* settings
) : Controller({"get", "options"}, settings),
	TemplateResponseMixin(settings->TEMPLATE_ENGINE.get())
{
}

TemplateController::TemplateController(
	const std::vector<std::string>& allowed_methods,
	conf::Settings* settings,
	const std::string& template_name,
	const std::string& content_type
) : Controller(allowed_methods, settings),
    TemplateResponseMixin(settings->TEMPLATE_ENGINE.get())
{
	this->template_name = template_name;
	this->content_type = content_type;
}

__CONTROLLERS_END__
