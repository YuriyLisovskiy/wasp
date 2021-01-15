/**
 * views/template_view.cpp
 *
 * Copyright (c) 2020 Yuriy Lisovskiy
 */

#include "./template_view.h"

// Framework libraries.
#include "../render/response.h"
#include "../conf/settings.h"


__VIEWS_BEGIN__

TemplateResponseMixin::TemplateResponseMixin(render::IEngine* engine)
{
	if (!engine)
	{
		throw core::ImproperlyConfigured(
			"Template engine must be initialized in order to use the application",
			_ERROR_DETAILS_
		);
	}

	this->engine = engine;
	this->template_name = "";
	this->content_type = "";
}

core::Result<std::shared_ptr<http::IHttpResponse>> TemplateResponseMixin::render(
	http::HttpRequest* request,
	const std::shared_ptr<render::IContext>& context,
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
	return core::Result<std::shared_ptr<http::IHttpResponse>>(response);
}

std::string TemplateResponseMixin::get_template_name()
{
	if (this->template_name.empty())
	{
		throw core::ImproperlyConfigured(
			"TemplateResponseMixin requires either a definition of '_template_name' or an "
			"implementation of 'get_template_name()'"
		);
	}

	return this->template_name;
}


TemplateView::TemplateView(
	conf::Settings* settings
) : views::View({"get", "options"}, settings),
	TemplateResponseMixin(settings->TEMPLATE_ENGINE.get())
{
}

TemplateView::TemplateView(
	const std::vector<std::string>& allowed_methods,
	conf::Settings* settings,
	const std::string& template_name,
	const std::string& content_type
) : views::View(allowed_methods, settings),
    TemplateResponseMixin(settings->TEMPLATE_ENGINE.get())
{
	this->template_name = template_name;
	this->content_type = content_type;
}

std::shared_ptr<render::IContext> TemplateView::get_context(
	http::HttpRequest* request, Args* args
)
{
	return nullptr;
}

core::Result<std::shared_ptr<http::IHttpResponse>> TemplateView::get(
	http::HttpRequest* request, Args* args
)
{
	return this->render(request, this->get_context(request, args));
}

__VIEWS_END__
