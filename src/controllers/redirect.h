/**
 * controllers/redirect.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * Provides a redirect on any request.
 */

#pragma once

// C++ libraries.
#include <memory>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "./controller.h"


__CONTROLLERS_BEGIN__

// TESTME: RedirectController
// TODO: docs for 'RedirectController'
class RedirectController : public Controller<>
{
private:
	std::string _url;
	bool _permanent;
	bool _query_string;

public:
	inline explicit RedirectController(
		conf::Settings* settings,
		std::string url,
		bool permanent=false,
		bool query_string=false
	) : Controller(
		{"get", "post", "put", "patch", "delete", "head", "options"},
		settings
	), _url(std::move(url)), _permanent(permanent), _query_string(query_string)
	{
	}

	// Return the URL redirect to.
	std::string get_redirect_url();

	std::unique_ptr<http::abc::IHttpResponse> get() override;

	inline std::unique_ptr<http::abc::IHttpResponse> post() override
	{
		return this->get();
	}

	inline std::unique_ptr<http::abc::IHttpResponse> head() override
	{
		return this->get();
	}

	inline std::unique_ptr<http::abc::IHttpResponse> options() override
	{
		return this->get();
	}

	inline std::unique_ptr<http::abc::IHttpResponse> put() override
	{
		return this->get();
	}

	inline std::unique_ptr<http::abc::IHttpResponse> delete_() override
	{
		return this->get();
	}

	inline std::unique_ptr<http::abc::IHttpResponse> patch() override
	{
		return this->get();
	}
};

__CONTROLLERS_END__
