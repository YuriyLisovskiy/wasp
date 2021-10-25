/**
 * controllers/template.h
 *
 * Copyright (c) 2020-2021 Yuriy Lisovskiy
 *
 * Classes for working with template responses.
 */

#pragma once

// Base libraries.
#include <xalwart.base/interfaces/render.h>

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
public:
	explicit TemplateResponseMixin(render::IEngine* engine);

	// Returns a response with a template rendered with
	// the given context.
	[[nodiscard]]
	virtual std::unique_ptr<http::HttpResponse> render(
		http::Request* request,
		const std::shared_ptr<render::IContext>& context,
		const std::string& template_name="",
		unsigned short int status=200,
		const std::string& content_type="",
		const std::string& charset="utf-8"
	) const;

	// Returns a template name to be used for the request.
	// May not be called if render() is overridden.
	[[nodiscard]]
	virtual std::string get_template_name() const;

protected:
	std::string template_name;
	std::string content_type;
	render::IEngine* engine;
};

// TESTME: TemplateController
// TODO: docs for 'TemplateController'
// A controller that can render a template.
template <typename ...UrlArgsT>
class TemplateController : public TemplateResponseMixin, public Controller<UrlArgsT...>
{
public:
	explicit TemplateController(const conf::Settings* settings) :
		Controller<UrlArgsT...>({"get", "options"}, settings),
		TemplateResponseMixin(settings ? settings->TEMPLATE_ENGINE.get() : nullptr)
	{
	}

	// Used in default get() method, can be overridden
	// in derived classes.
	[[nodiscard]]
	virtual inline std::shared_ptr<render::IContext> get_context(http::Request* request, UrlArgsT ...args) const
	{
		return nullptr;
	}

	[[nodiscard]]
	inline std::unique_ptr<http::HttpResponse> get(http::Request* request, UrlArgsT ...args) const override
	{
		return this->render(request, this->get_context(request, args...), "", 200, "", "utf-8");
	}

protected:
	explicit TemplateController(
		const std::vector<std::string>& allowed_methods,
		const conf::Settings* settings,
		const std::string& template_name="",
		const std::string& content_type=""
	) : Controller<UrlArgsT...>(allowed_methods, settings),
		TemplateResponseMixin(settings ? settings->TEMPLATE_ENGINE.get() : nullptr)
	{
		this->template_name = template_name;
		this->content_type = content_type;
	}
};

__CONTROLLERS_END__
