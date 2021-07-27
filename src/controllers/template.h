/**
 * controllers/template.h
 *
 * Copyright (c) 2020-2021 Yuriy Lisovskiy
 *
 * Classes for working with template responses.
 */

#pragma once

// Render libraries.
#include <xalwart.render/abc.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "./controller.h"


__CONTROLLERS_BEGIN__

/// A mixin that can be used to render a template.
class TemplateResponseMixin
{
protected:
	std::string template_name;
	std::string content_type;
	render::abc::IEngine* engine;

public:
	explicit TemplateResponseMixin(render::abc::IEngine* engine);

	/// Returns a response with a template rendered with
	/// the given context.
	virtual Result<std::shared_ptr<http::IHttpResponse>> render(
		http::HttpRequest* request,
		const std::shared_ptr<render::abc::IContext>& context,
		const std::string& template_name="",
		unsigned short int status=200,
		const std::string& content_type="",
		const std::string& charset="utf-8"
	);

	/// Returns a template name to be used for the request.
	/// May not be called if render() is overridden.
	virtual std::string get_template_name();
};


/// A controller that can render a template.
class TemplateController : public TemplateResponseMixin, public Controller
{
public:
	explicit TemplateController(conf::Settings* settings);

	/// Used in default get() method, can be overridden
	/// in derived classes.
	virtual inline std::shared_ptr<render::abc::IContext> get_context(core::Kwargs* kwargs)
	{
		return nullptr;
	}

	inline Result<std::shared_ptr<http::IHttpResponse>> get(core::Kwargs* kwargs) override
	{
		return this->render(this->request, this->get_context(kwargs), "", 200, "", "utf-8");
	}

protected:
	explicit TemplateController(
		const std::vector<std::string>& allowed_methods,
		conf::Settings* settings,
		const std::string& template_name="",
		const std::string& content_type=""
	);
};

__CONTROLLERS_END__
