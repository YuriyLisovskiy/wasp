/**
 * controllers/redirect.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * Provides a redirect on any request.
 */

#pragma once

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "./controller.h"


__CONTROLLERS_BEGIN__

class RedirectController : public Controller
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

	/// Return the URL redirect to.
	std::string get_redirect_url();

	Result<std::shared_ptr<http::IHttpResponse>> get(core::Kwargs* args) override;
	Result<std::shared_ptr<http::IHttpResponse>> post(core::Kwargs* args) override;
	Result<std::shared_ptr<http::IHttpResponse>> head(core::Kwargs* args) override;
	Result<std::shared_ptr<http::IHttpResponse>> options(core::Kwargs* args) override;
	Result<std::shared_ptr<http::IHttpResponse>> put(core::Kwargs* args) override;
	Result<std::shared_ptr<http::IHttpResponse>> delete_(core::Kwargs* args) override;
	Result<std::shared_ptr<http::IHttpResponse>> patch(core::Kwargs* args) override;
};

__CONTROLLERS_END__
