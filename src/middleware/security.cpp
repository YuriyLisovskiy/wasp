/**
 * middleware/security.cpp
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 */

#include "./security.h"

// Base libraries.
#include <xalwart.base/string_utils.h>

// Framework libraries.
#include "../http/headers.h"


__MIDDLEWARE_BEGIN__

Security::Security(conf::Settings* settings) : BaseMiddleware(settings)
{
	this->sts_seconds = settings->SECURE_HSTS_SECONDS;
	this->sts_include_subdomains = settings->SECURE_HSTS_INCLUDE_SUBDOMAINS;
	this->sts_preload = settings->SECURE_HSTS_PRELOAD;
	this->content_type_no_sniff = settings->SECURE_CONTENT_TYPE_NO_SNIFF;
	this->xss_filter = settings->SECURE_BROWSER_XSS_FILTER;
	this->redirect = settings->SECURE_SSL_REDIRECT;
	this->redirect_host = settings->SECURE_SSL_HOST;
	this->referrer_policy = settings->SECURE_REFERRER_POLICY;
	for (auto& pattern : settings->SECURE_REDIRECT_EXEMPT)
	{
		this->redirect_exempt.emplace_back(pattern);
	}
}

http::Response::Result Security::process_request(http::Request* request)
{
	auto path = str::ltrim(request->url.path, "/");
	bool matched = false;
	for (auto& pattern : this->redirect_exempt)
	{
		matched = pattern.search(path);
		if (matched)
		{
			break;
		}
	}

	if (
		this->redirect &&
		!request->is_secure(this->settings->SECURE_PROXY_SSL_HEADER) &&
		!matched
	)
	{
		std::string host;
		if (this->redirect_host.empty())
		{
			host = request->get_host(
				this->settings->SECURE_PROXY_SSL_HEADER,
				this->settings->USE_X_FORWARDED_HOST,
				this->settings->USE_X_FORWARDED_PORT,
				this->settings->DEBUG,
				this->settings->ALLOWED_HOSTS
			);
		}
		else
		{
			host = this->redirect_host;
		}

		return http::result<http::resp::PermanentRedirect>("https://" + host + request->url.full_path());
	}

	return {};
}

http::Response::Result Security::process_response(http::Request* request, http::abc::IHttpResponse* response)
{
	if (
		this->sts_seconds &&
		request->is_secure(this->settings->SECURE_PROXY_SSL_HEADER) &&
		!response->has_header(http::STRICT_TRANSPORT_SECURITY)
	)
	{
		auto sts_header = "max-age=" + std::to_string(this->sts_seconds);
		if (this->sts_include_subdomains)
		{
			sts_header += "; includeSubDomains";
		}

		if (this->sts_preload)
		{
			sts_header += "; preload";
		}

		response->set_header(http::STRICT_TRANSPORT_SECURITY, sts_header);
	}

	if (this->content_type_no_sniff)
	{
		response->set_header(http::X_CONTENT_TYPE_OPTIONS, "nosniff");
	}

	if (this->xss_filter)
	{
		response->set_header(http::X_XSS_PROTECTION, "1; mode=block");
	}

	if (!this->referrer_policy.empty())
	{
		// Support a comma-separated string or iterable of
		// values to allow fallback.
		auto split = str::split(this->referrer_policy, ',');
		for (auto& item : split)
		{
			item = str::trim(item);
		}

		response->set_header(
			http::REFERRER_POLICY, str::join(",", split.begin(), split.end())
		);
	}

	return {};
}

__MIDDLEWARE_END__
