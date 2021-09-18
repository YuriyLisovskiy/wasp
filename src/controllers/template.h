/**
 * controllers/template.h
 *
 * Copyright (c) 2020-2021 Yuriy Lisovskiy
 *
 * Classes for working with template responses.
 */

#pragma once

// Base libraries.
#include <xalwart.base/abc/render.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "./controller.h"
#include "../conf/settings.h"


__CONTROLLERS_BEGIN__

// TESTME: TemplateResponseMixin
// TODO: docs for 'TemplateResponseMixin'
// A mixin that can be used to render a template.
class TemplateResponseMixin
{
protected:
	std::string template_name;
	std::string content_type;
	abc::render::IEngine* engine;

public:
	explicit TemplateResponseMixin(abc::render::IEngine* engine);

	// Returns a response with a template rendered with
	// the given context.
	virtual std::unique_ptr<http::abc::IHttpResponse> render(
		http::Request* request,
		const std::shared_ptr<abc::render::IContext>& context,
		const std::string& template_name="",
		unsigned short int status=200,
		const std::string& content_type="",
		const std::string& charset="utf-8"
	);

	// Returns a template name to be used for the request.
	// May not be called if render() is overridden.
	virtual std::string get_template_name();
};

// TESTME: TemplateController
// TODO: docs for 'TemplateController'
// A controller that can render a template.
template <typename ...UrlArgsT>
class TemplateController : public TemplateResponseMixin, public Controller<UrlArgsT...>
{
public:
	explicit TemplateController(conf::Settings* settings) :
		Controller<UrlArgsT...>({"get", "options"}, settings),
		TemplateResponseMixin(settings->TEMPLATE_ENGINE.get())
	{
	}

	// Used in default get() method, can be overridden
	// in derived classes.
	virtual inline std::shared_ptr<abc::render::IContext> get_context(UrlArgsT ...args)
	{
		return nullptr;
	}

	inline std::unique_ptr<http::abc::IHttpResponse> get(UrlArgsT ...args) override
	{
		return this->render(this->request, this->get_context(args...), "", 200, "", "utf-8");
	}

protected:
	explicit TemplateController(
		const std::vector<std::string>& allowed_methods,
		conf::Settings* settings,
		const std::string& template_name="",
		const std::string& content_type=""
	) : Controller<UrlArgsT...>(allowed_methods, settings), TemplateResponseMixin(settings->TEMPLATE_ENGINE.get())
	{
		this->template_name = template_name;
		this->content_type = content_type;
	}
};

__CONTROLLERS_END__
