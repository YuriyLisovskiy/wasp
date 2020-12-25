/**
 * middleware/security.cpp
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 */

#include "./security.h"

// Core libraries.
#include <xalwart.core/string_utils.h>

// Framework libraries.
#include "../http/headers.h"


__MIDDLEWARE_BEGIN__

const std::string SecurityMiddleware::FULL_NAME = "xw::middleware::SecurityMiddleware";

SecurityMiddleware::SecurityMiddleware(
	conf::Settings* settings
) : MiddlewareMixin(settings)
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

http::Result<std::shared_ptr<http::IHttpResponse>> SecurityMiddleware::process_request(
	http::HttpRequest* request
)
{
	auto path = core::str::ltrim(request->path(), "/");
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
		!request->is_secure(this->settings->SECURE_PROXY_SSL_HEADER.get()) &&
		!matched
	)
	{
		std::string host;
		if (this->redirect_host.empty())
		{
			auto result = request->get_host(
				this->settings->USE_X_FORWARDED_HOST,
				this->settings->DEBUG,
				this->settings->ALLOWED_HOSTS
			);
			if (result.err)
			{
				return result.forward<std::shared_ptr<http::IHttpResponse>>();
			}

			host = result.value;
		}
		else
		{
			host = this->redirect_host;
		}

		return this->result<http::HttpResponsePermanentRedirect, std::string>(
			"https://" + host + request->full_path()
		);
	}

	return this->none();
}

http::Result<std::shared_ptr<http::IHttpResponse>> SecurityMiddleware::process_response(
	http::HttpRequest* request, http::IHttpResponse* response
)
{
	if (
		this->sts_seconds &&
		request->is_secure(this->settings->SECURE_PROXY_SSL_HEADER.get()) &&
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
		auto split = core::str::split(this->referrer_policy, ',');
		for (auto& item : split)
		{
			core::str::trim(item);
		}

		response->set_header(
			http::REFERRER_POLICY, core::str::join(split.begin(), split.end(), ",")
		);
	}

	return this->none();
}

__MIDDLEWARE_END__
