/**
 * middleware/security.cpp
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 */

#include "./security.h"

// Base libraries.
#include <xalwart.base/string_utils.h>


__MIDDLEWARE_BEGIN__

Function Security::operator() (const Function& next) const
{
	return [*this, next](http::Request* request) -> std::unique_ptr<http::abc::IHttpResponse>
	{
		auto response = this->preprocess(request);
		if (response)
		{
			return response;
		}

		response = next(request);
		auto postprocess_response = this->postprocess(request, response.get());
		if (postprocess_response)
		{
			return postprocess_response;
		}

		return response;
	};
}

std::unique_ptr<http::abc::IHttpResponse> Security::preprocess(http::Request* request) const
{
	auto path = str::ltrim(request->url.path, "/");
	bool matched = false;
	for (auto pattern : this->redirect_exempt)
	{
		matched = pattern.search(path);
		if (matched)
		{
			break;
		}
	}

	if (
		this->secure.SSL_REDIRECT &&
		!request->is_secure(this->secure.PROXY_SSL_HEADER) &&
		!matched
	)
	{
		std::string host;
		if (this->secure.SSL_HOST.empty())
		{
			host = request->get_host(
				this->secure.PROXY_SSL_HEADER,
				this->settings->USE_X_FORWARDED_HOST,
				this->settings->USE_X_FORWARDED_PORT,
				this->settings->DEBUG,
				this->settings->ALLOWED_HOSTS
			);
		}
		else
		{
			host = this->secure.SSL_HOST;
		}

		return std::make_unique<http::resp::PermanentRedirect>("https://" + host + request->url.full_path());
	}

	return nullptr;
}

std::unique_ptr<http::abc::IHttpResponse> Security::postprocess(
	http::Request* request, http::abc::IHttpResponse* response
) const
{
	require_non_null(response, _ERROR_DETAILS_);
	if (
		this->secure.HSTS_SECONDS &&
		request->is_secure(this->secure.PROXY_SSL_HEADER) &&
		!response->has_header(http::STRICT_TRANSPORT_SECURITY)
	)
	{
		auto sts_header = "max-age=" + std::to_string(this->secure.HSTS_SECONDS);
		if (this->secure.HSTS_INCLUDE_SUBDOMAINS)
		{
			sts_header += "; includeSubDomains";
		}

		if (this->secure.HSTS_PRELOAD)
		{
			sts_header += "; preload";
		}

		response->set_header(http::STRICT_TRANSPORT_SECURITY, sts_header);
	}

	if (this->secure.CONTENT_TYPE_NO_SNIFF)
	{
		response->set_header(http::X_CONTENT_TYPE_OPTIONS, "nosniff");
	}

	if (this->secure.BROWSER_XSS_FILTER)
	{
		response->set_header(http::X_XSS_PROTECTION, "1; mode=block");
	}

	if (!this->secure.REFERRER_POLICY.empty())
	{
		// Support a comma-separated string or iterable of
		// values to allow fallback.
		auto split = str::split(this->secure.REFERRER_POLICY, ',');
		for (auto& item : split)
		{
			item = str::trim(item);
		}

		response->set_header(http::REFERRER_POLICY, str::join(",", split.begin(), split.end()));
	}

	return nullptr;
}

__MIDDLEWARE_END__
