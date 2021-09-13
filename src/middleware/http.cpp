/**
 * middleware/http.cpp
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 */

#include "./http.h"

// Base libraries.
#include <xalwart.base/string_utils.h>

// Framework libraries.
#include "../utility/cache.h"


__MIDDLEWARE_BEGIN__

Function ConditionalGet::operator() (const Function& next) const
{
	return [*this, next](http::Request* request) -> std::unique_ptr<http::abc::IHttpResponse>
	{
		auto response = next(request);

		// It's too late to prevent an unsafe request with a 412 response, and
		// for a HEAD request, the response body is always empty so computing
		// an accurate ETag isn't possible.
		if (request->method != "GET")
		{
			return response;
		}

		if (this->needs_etag(response) && !response->has_header(http::E_TAG))
		{
			util::cache::set_response_etag(response.get());
		}

		auto etag = response->get_header(http::E_TAG, "");
		long last_modified = http::parse_http_date(response->get_header(http::LAST_MODIFIED, ""));
		if (!etag.empty() || last_modified != -1)
		{
			auto conditional_response = util::cache::get_conditional_response(
				request, etag, last_modified, response.get()
			);
			if (conditional_response)
			{
				return conditional_response;
			}
		}

		return response;
	};
}

bool ConditionalGet::needs_etag(const std::unique_ptr<http::abc::IHttpResponse>& response) const
{
	auto cache_control = str::split(response->get_header(http::CACHE_CONTROL, ""), ',');
	bool result = true;
	for (const auto& directive : cache_control)
	{
		if (str::to_lower(str::trim(directive)) == "no-store")
		{
			result = false;
			break;
		}
	}

	return result;
}

__MIDDLEWARE_END__
