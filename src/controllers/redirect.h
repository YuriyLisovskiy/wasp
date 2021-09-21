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
		const conf::Settings* settings,
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
	[[nodiscard]]
	std::string get_redirect_url(http::Request* request) const;

	[[nodiscard]]
	std::unique_ptr<http::abc::HttpResponse> get(http::Request* request) const override;

	[[nodiscard]]
	inline std::unique_ptr<http::abc::HttpResponse> post(http::Request* request) const override
	{
		return this->get(request);
	}

	[[nodiscard]]
	inline std::unique_ptr<http::abc::HttpResponse> head(http::Request* request) const override
	{
		return this->get(request);
	}

	[[nodiscard]]
	inline std::unique_ptr<http::abc::HttpResponse> options(http::Request* request) const override
	{
		return this->get(request);
	}

	[[nodiscard]]
	inline std::unique_ptr<http::abc::HttpResponse> put(http::Request* request) const override
	{
		return this->get(request);
	}

	[[nodiscard]]
	inline std::unique_ptr<http::abc::HttpResponse> delete_(http::Request* request) const override
	{
		return this->get(request);
	}

	[[nodiscard]]
	inline std::unique_ptr<http::abc::HttpResponse> patch(http::Request* request) const override
	{
		return this->get(request);
	}
};

__CONTROLLERS_END__
