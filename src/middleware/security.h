/**
 * middleware/security.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * TODO: docs
 */

#pragma once

// C++ libraries.
#include <vector>
#include <string>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "./base.h"


__MIDDLEWARE_BEGIN__

// TESTME: SecurityMiddleware
// TODO: docs for 'SecurityMiddleware'
class SecurityMiddleware : public BaseMiddleware
{
public:
	inline static const std::string FULL_NAME = "xw::middleware::SecurityMiddleware";

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

	http::result_t process_request(http::HttpRequest* request) override;

	http::result_t process_response(http::HttpRequest* request, http::IHttpResponse* response) override;
};

__MIDDLEWARE_END__
