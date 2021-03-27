/**
 * middleware/security.h
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 *
 * Purpose: TODO
 */

#pragma once

// C++ libraries.
#include <vector>
#include <string>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "./middleware_mixin.h"


__MIDDLEWARE_BEGIN__

class SecurityMiddleware : public MiddlewareMixin
{
public:
	static const std::string FULL_NAME;

protected:
	size_t sts_seconds;
	bool sts_include_subdomains;
	bool sts_preload;
	bool content_type_no_sniff;
	bool xss_filter;
	bool redirect;
	std::string redirect_host;
	std::string referrer_policy;
	std::vector<re::Regex> redirect_exempt;

public:
	explicit SecurityMiddleware(conf::Settings* settings);

	core::Result<std::shared_ptr<http::IHttpResponse>> process_request(
		http::HttpRequest* request
	) override;
	core::Result<std::shared_ptr<http::IHttpResponse>> process_response(
		http::HttpRequest* request, http::IHttpResponse* response
	) override;
};

__MIDDLEWARE_END__
