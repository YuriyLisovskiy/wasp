/**
 * views/template_view.h
 *
 * Copyright (c) 2020 Yuriy Lisovskiy
 *
 * Purpose: classes for working with template responses.
 */

#pragma once

// Render libraries.
#include <xalwart.render/base.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "../views/view.h"


__VIEWS_BEGIN__

/// A mixin that can be used to render a template.
class TemplateResponseMixin
{
protected:
	std::string template_name;
	std::string content_type;
	render::IEngine* engine;

public:
	explicit TemplateResponseMixin(render::IEngine* engine);

	/// Returns a response with a template rendered with
	/// the given context.
	virtual http::Result<std::shared_ptr<http::IHttpResponse>> render(
		http::HttpRequest* request,
		const std::shared_ptr<render::IContext>& context = nullptr,
		const std::string& template_name = "",
		unsigned short int status = 200,
		const std::string& content_type = "",
		const std::string& charset = "utf-8"
	);

	/// Returns a template name to be used for the request.
	/// May not be called if render() is overridden.
	virtual std::string get_template_name();
};


/// A view that can render a template.
class TemplateView : public TemplateResponseMixin, public View
{
public:
	explicit TemplateView(conf::Settings* settings);

	/// Used in default get() method, can be overridden
	/// in derived classes.
	virtual std::shared_ptr<render::IContext> get_context(
		http::HttpRequest* request, Args* args
	);

	http::Result<std::shared_ptr<http::IHttpResponse>> get(
		http::HttpRequest* request, Args* args
	) override;

protected:
	explicit TemplateView(
		const std::vector<std::string>& allowed_methods,
		conf::Settings* settings,
		const std::string& template_name = "",
		const std::string& content_type = ""
	);
};

__VIEWS_END__
