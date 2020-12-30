/**
 * views/redirect_view.h
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 *
 * Purpose: provides a redirect on any GET request.
 */

#pragma once

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "./view.h"


__VIEWS_BEGIN__

class RedirectView : public View
{
private:
	std::string _url;
	bool _permanent;
	bool _query_string;

public:
	explicit RedirectView(
		conf::Settings* settings,
		const std::string& url,
		bool permanent = false,
		bool query_string = false
	);

	/// Return the URL redirect to.
	std::string get_redirect_url();

	core::Result<std::shared_ptr<http::IHttpResponse>> get(http::HttpRequest* request, Args* args) override;
	core::Result<std::shared_ptr<http::IHttpResponse>> post(http::HttpRequest* request, Args* args) override;
	core::Result<std::shared_ptr<http::IHttpResponse>> head(http::HttpRequest* request, Args* args) override;
	core::Result<std::shared_ptr<http::IHttpResponse>> options(http::HttpRequest* request, Args* args) override;
	core::Result<std::shared_ptr<http::IHttpResponse>> put(http::HttpRequest* request, Args* args) override;
	core::Result<std::shared_ptr<http::IHttpResponse>> delete_(http::HttpRequest* request, Args* args) override;
	core::Result<std::shared_ptr<http::IHttpResponse>> patch(http::HttpRequest* request, Args* args) override;
};

__VIEWS_END__
