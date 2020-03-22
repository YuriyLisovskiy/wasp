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
 * views/template_view.h
 *
 * Purpose:
 * Classes for working with template responses.
 */

#pragma once

// C++ libraries.
#include <string>
#include <memory>

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "../http/request.h"
#include "../http/response.h"
#include "../render/base.h"
#include "../render/response.h"
#include "../render/env/interfaces.h"
#include "../core/logger.h"
#include "../core/exceptions.h"
#include "../views/view.h"
#include "../conf/settings.h"
#include "../views/args.h"


__VIEWS_BEGIN__

/// A mixin that can be used to render a template.
class TemplateResponseMixin
{
protected:
	std::string _template_name;
	std::string _content_type;
	render::env::IEnvironment* _env;

public:
	explicit TemplateResponseMixin(
		render::env::IEnvironment* env
	);

	/// Returns a response with a template rendered with
	/// the given context.
	virtual std::unique_ptr<http::IHttpResponse> render(
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

	std::unique_ptr<http::IHttpResponse> get(
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
