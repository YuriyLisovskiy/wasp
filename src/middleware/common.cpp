/**
 * middleware/common.cpp
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 */

#include "./common.h"

// Base libraries.
#include <xalwart.base/utility.h>

// Framework libraries.
#include "../urls/resolver.h"
#include "../http/headers.h"
#include "../http/utility.h"
#include "../http/exceptions.h"


__MIDDLEWARE_BEGIN__

bool Common::should_redirect_with_slash(http::Request* request)
{
	if (this->settings->APPEND_SLASH && !request->path().ends_with("/"))
	{
		auto path = request->path();
		return !urls::is_valid_path(path, this->settings->URLPATTERNS) &&
			urls::is_valid_path(path + "/", this->settings->URLPATTERNS);
	}

	return false;
}

std::pair<std::string, std::shared_ptr<BaseException>> Common::get_full_path_with_slash(http::Request* request)
{
	auto new_path = request->full_path(true);

	// Prevent construction of scheme relative urls.
	http::escape_leading_slashes(new_path);
	if (this->settings->DEBUG && util::contains<std::string>(
		request->method(), {"POST", "PUT", "PATCH"}
	))
	{
		auto method = request->method();
		auto result = request->get_host(
			this->settings->USE_X_FORWARDED_HOST,
			this->settings->USE_X_FORWARDED_PORT,
			this->settings->DEBUG,
			this->settings->ALLOWED_HOSTS
		);
		if (result.second)
		{
			this->settings->LOGGER->trace("Method 'get_host' returned an error", _ERROR_DETAILS_);
			return result;
		}

		auto host = result.first;
		throw RuntimeError(
			"You called this URL via " + method + "s, but the URL doesn't end "
			"in a slash and you have APPEND_SLASH set. Xalwart can't "
			"redirect to the slash URL while maintaining " + method + "s data. "
			"Change your form to point to " + host + new_path + "s (note the trailing "
			"slash), or set APPEND_SLASH to false in your Xalwart settings."
		);
	}

	return {new_path, nullptr};
}

http::result_t Common::process_request(http::Request* request)
{
	if (request->headers.contains(http::USER_AGENT))
	{
		auto user_agent = request->headers.get(http::USER_AGENT);
		for (auto& rgx : this->settings->DISALLOWED_USER_AGENTS)
		{
			if (rgx.search(user_agent))
			{
				this->settings->LOGGER->trace(
					"Found user agent which is not allowed: '" + user_agent + "'", _ERROR_DETAILS_
				);
				return http::raise(403, "Forbidden user agent");
			}
		}
	}

	// Check for a redirect based on settings.PREPEND_WWW
	auto result = request->get_host(
		this->settings->USE_X_FORWARDED_HOST,
		this->settings->USE_X_FORWARDED_PORT,
		this->settings->DEBUG,
		this->settings->ALLOWED_HOSTS
	);
	if (result.second)
	{
		this->settings->LOGGER->trace("Method 'get_host' returned an error", _ERROR_DETAILS_);
		return {nullptr, result.second};
	}

	auto host = result.first;
	bool must_prepend = this->settings->PREPEND_WWW && !host.empty() && !host.starts_with("www.");
	auto redirect_url = must_prepend ? (
		request->scheme(this->settings->SECURE_PROXY_SSL_HEADER.get()) + "://www." + host
	) : "";

	// Check if a slash should be appended.
	std::string path;
	if (this->should_redirect_with_slash(request))
	{
		result = this->get_full_path_with_slash(request);
		if (result.second)
		{
			this->settings->LOGGER->trace("Method 'get_full_path_with_slash' returned an error", _ERROR_DETAILS_);
			return {nullptr, result.second};
		}

		path = result.first;
	}
	else
	{
		path = request->full_path();
	}

	// Return a redirect if necessary.
	if (!redirect_url.empty() || path != request->full_path())
	{
		redirect_url += path;
		return this->get_response_redirect(redirect_url);
	}

	return {};
}

http::result_t Common::process_response(http::Request* request, http::abc::IHttpResponse* response)
{
	// If the given URL is "Not Found", then check if we
	// should redirect to a path with a slash appended.
	if (response->status() == 404)
	{
		if (this->should_redirect_with_slash(request))
		{
			auto result = this->get_full_path_with_slash(request);
			if (result.second)
			{
				this->settings->LOGGER->trace("Method 'get_full_path_with_slash' returned an error", _ERROR_DETAILS_);
				return {nullptr, result.second};
			}

			return this->get_response_redirect(result.first);
		}
	}

	// Add the Content-Length header to non-streaming
	// responses if not already set.
	if (!response->is_streaming() && !response->has_header(http::CONTENT_LENGTH))
	{
		response->set_header(http::CONTENT_LENGTH, std::to_string(response->content_length()));
	}

	return {};
}

__MIDDLEWARE_END__
