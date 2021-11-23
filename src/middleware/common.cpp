/**
 * middleware/common.cpp
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 */

#include "./common.h"

// C++ libraries.
#include <memory>

// Base libraries.
#include <xalwart.base/utility.h>

// Framework libraries.
#include "../urls/resolver.h"


__MIDDLEWARE_BEGIN__

Function Common::operator() (const Function& next) const
{
	return [*this, next](http::IRequest* request) -> std::unique_ptr<http::IResponse>
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

bool Common::should_redirect_with_slash(http::IRequest* request) const
{
	const auto& request_url = request->url();
	if (this->settings->APPEND_SLASH && !request_url.path.ends_with("/"))
	{
		return !urls::is_valid_path(request_url.path, this->settings->URLPATTERNS) &&
			urls::is_valid_path(request_url.path + "/", this->settings->URLPATTERNS);
	}

	return false;
}

std::string Common::get_full_path_with_slash(http::IRequest* request) const
{
	auto new_path = request->url().full_path(true);

	// Prevent construction of scheme relative urls.
	http::escape_leading_slashes(new_path);
	auto method = request->method();
	if (this->settings->DEBUG && util::contains<std::string>(method, {"POST", "PUT", "PATCH"}))
	{
		auto host = request->get_host(
			this->settings->SECURE.PROXY_SSL_HEADER,
			this->settings->USE_X_FORWARDED_HOST,
			this->settings->USE_X_FORWARDED_PORT,
			this->settings->DEBUG,
			this->settings->ALLOWED_HOSTS
		);
		throw RuntimeError(
			"You called this URL via " + method + "s, but the URL doesn't end "
			"in a slash and 'settings->APPEND_SLASH' is 'true'. " + v::framework_name + " can't "
			"redirect to the slash URL while maintaining " + method + "s data. "
			"Change your form to point to " + host + new_path + "s (note the trailing "
			"slash), or set 'settings->APPEND_SLASH' to 'false' in your Xalwart settings."
		);
	}

	return new_path;
}

std::unique_ptr<http::IResponse> Common::preprocess(http::IRequest* request) const
{
	require_non_null(request, _ERROR_DETAILS_);
	if (request->has_header(http::USER_AGENT))
	{
		auto user_agent = request->get_header(http::USER_AGENT, "");
		for (const auto& rgx : this->settings->DISALLOWED_USER_AGENTS)
		{
			auto to_search = rgx;
			if (to_search.search(user_agent))
			{
				require_non_null(this->settings->LOGGER.get(), _ERROR_DETAILS_)->trace(
					"Found user agent which is not allowed: '" + user_agent + "'", _ERROR_DETAILS_
				);
				return std::make_unique<http::Forbidden>("Forbidden user agent");
			}
		}
	}

	// Check for a redirect based on `settings->PREPEND_WWW`
	auto host = request->get_host(
		this->settings->SECURE.PROXY_SSL_HEADER,
		this->settings->USE_X_FORWARDED_HOST,
		this->settings->USE_X_FORWARDED_PORT,
		this->settings->DEBUG,
		this->settings->ALLOWED_HOSTS
	);
	bool must_prepend = this->settings->PREPEND_WWW && !host.empty() && !host.starts_with("www.");
	std::string scheme = request->scheme(this->settings->SECURE.PROXY_SSL_HEADER);
	auto redirect_url = must_prepend ? (scheme + "://www." + host) : "";

	// Check if a slash should be appended.
	std::string path;
	if (this->should_redirect_with_slash(request))
	{
		path = this->get_full_path_with_slash(request);
	}
	else
	{
		path = request->url().full_path();
	}

	// Return a redirect if necessary.
	if (!redirect_url.empty() || path != request->url().full_path())
	{
		redirect_url += path;
		return this->get_response_redirect(redirect_url);
	}

	return nullptr;
}

std::unique_ptr<http::IResponse> Common::postprocess(
	http::IRequest* request, http::IResponse* response
) const
{
	require_non_null(response, _ERROR_DETAILS_);

	// If the given URL is "Not Found", then check if we
	// should redirect to a path with a slash appended.
	if (response->get_status() == 404)
	{
		if (this->should_redirect_with_slash(request))
		{
			return this->get_response_redirect(this->get_full_path_with_slash(request));
		}
	}

	if (response->get_charset().empty())
	{
		response->set_charset(this->settings->CHARSET);
	}

	// Add the Content-Length header to non-streaming
	// responses if not already set.
	if (!response->is_streaming() && !response->has_header(http::CONTENT_LENGTH))
	{
		response->set_header(http::CONTENT_LENGTH, std::to_string(response->content_length()));
	}

	return nullptr;
}

__MIDDLEWARE_END__
