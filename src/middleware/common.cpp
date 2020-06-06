/*
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * An implementation of middleware/common.h
 */

#include "./common.h"

// Framework modules.
#include "../urls/resolver.h"
#include "../http/headers.h"
#include "../http/utility.h"
#include "../core/strings.h"


__MIDDLEWARE_BEGIN__

const std::string CommonMiddleware::FULL_NAME = "xw::middleware::CommonMiddleware";

CommonMiddleware::CommonMiddleware(conf::Settings* settings)
	: MiddlewareMixin(settings)
{
}

std::unique_ptr<http::IHttpResponse> CommonMiddleware::get_response_redirect(
	const std::string& redirect_to
)
{
	return std::make_unique<http::HttpResponsePermanentRedirect>(redirect_to);
}

bool CommonMiddleware::should_redirect_with_slash(http::HttpRequest* request)
{
	if (this->settings->APPEND_SLASH && !core::str::ends_with(request->path(), "/"))
	{
		auto path = request->path();
		return !urls::is_valid_path(path, this->settings->ROOT_URLCONF) &&
				urls::is_valid_path(path + "/", this->settings->ROOT_URLCONF);
	}

	return false;
}

std::string CommonMiddleware::get_full_path_with_slash(http::HttpRequest* request)
{
	auto new_path = request->full_path(true);

	// Prevent construction of scheme relative urls.
	http::escape_leading_slashes(new_path);
	if (this->settings->DEBUG && core::utility::contains(
		request->method(), {"POST", "PUT", "PATCH"}
	))
	{
		auto method = request->method();
		auto host = request->get_host(
			this->settings->USE_X_FORWARDED_HOST,
			this->settings->DEBUG,
			this->settings->ALLOWED_HOSTS
		);
		throw core::RuntimeError(
			"You called this URL via " + method + "s, but the URL doesn't end "
			"in a slash and you have APPEND_SLASH set. Django can't "
			"redirect to the slash URL while maintaining " + method + "s data. "
			"Change your form to point to " + host + new_path + "s (note the trailing "
			"slash), or set APPEND_SLASH=False in your Django settings."
		);
	}

	return new_path;
}

std::unique_ptr<http::IHttpResponse> CommonMiddleware::process_request(
	http::HttpRequest* request
)
{
	if (request->headers.contains(http::USER_AGENT))
	{
		auto user_agent = request->headers.get(http::USER_AGENT);
		for (auto& rgx : this->settings->DISALLOWED_USER_AGENTS)
		{
			if (rgx.search(user_agent))
			{
				throw core::PermissionDenied("Forbidden user agent", _ERROR_DETAILS_);
			}
		}
	}

	// Check for a redirect based on settings.PREPEND_WWW
	auto host = request->get_host(
		this->settings->USE_X_FORWARDED_HOST,
		this->settings->DEBUG,
		this->settings->ALLOWED_HOSTS
	);
	bool must_prepend = this->settings->PREPEND_WWW &&
		!host.empty() &&
		!core::str::starts_with(host, "www.");
	auto redirect_url = must_prepend ? (
		request->scheme(this->settings->SECURE_PROXY_SSL_HEADER.get()) + "://www." + host
	) : "";

	// Check if a slash should be appended.
	std::string path;
	if (this->should_redirect_with_slash(request))
	{
		path = this->get_full_path_with_slash(request);
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

	return nullptr;
}

std::unique_ptr<http::IHttpResponse> CommonMiddleware::process_response(
	http::HttpRequest* request, http::IHttpResponse* response
)
{
	// If the given URL is "Not Found", then check if we
	// should redirect to a path with a slash appended.
	if (response->status() == 404)
	{
		if (this->should_redirect_with_slash(request))
		{
			return this->get_response_redirect(
				this->get_full_path_with_slash(request)
			);
		}
	}

	// Add the Content-Length header to non-streaming
	// responses if not already set.
	if (!response->is_streaming() && !response->has_header(http::CONTENT_LENGTH))
	{
		response->set_header(
			http::CONTENT_LENGTH, std::to_string(response->content_length())
		);
	}

	return nullptr;
}

__MIDDLEWARE_END__
