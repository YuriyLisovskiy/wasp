/**
 * middleware/http.cpp
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 */

#include "./http.h"

// Core libraries.
#include <xalwart.core/string_utils.h>

// Framework libraries.
#include "../utils/cache.h"
#include "../utils/http.h"
#include "../http/headers.h"


__MIDDLEWARE_BEGIN__

const std::string ConditionalGetMiddleware::FULL_NAME = "xw::middleware::ConditionalGetMiddleware";

ConditionalGetMiddleware::ConditionalGetMiddleware(
	conf::Settings* settings
) : MiddlewareMixin(settings)
{
}

Result<std::shared_ptr<http::IHttpResponse>> ConditionalGetMiddleware::process_response(
	http::HttpRequest* request, http::IHttpResponse* response
)
{
	/// It's too late to prevent an unsafe request with a 412 response, and
	/// for a HEAD request, the response body is always empty so computing
	/// an accurate ETag isn't possible.
	if (request->method() != "GET")
	{
		return this->none();
	}

	if (needs_etag(response) && !response->has_header(http::E_TAG))
	{
		utils::cache::set_response_etag(response);
	}

	auto etag = response->get_header(http::E_TAG, "");
	long last_modified = utils_http::parse_http_date(
		response->get_header(http::LAST_MODIFIED, "")
	);
	if (!etag.empty() || last_modified != -1)
	{
		auto conditional_response = utils::cache::get_conditional_response(
			request, etag, last_modified, response
		);
		if (conditional_response)
		{
			return this->result(conditional_response);
		}
	}

	return this->none();
}

bool ConditionalGetMiddleware::needs_etag(
	http::IHttpResponse* response
)
{
	auto cache_control = str::split(
		response->get_header(http::CACHE_CONTROL, ""), ','
	);
	bool result = true;
	for (const auto& directive : cache_control)
	{
		if (str::lower(str::trim(directive)) == "no-store")
		{
			result = false;
			break;
		}
	}

	return result;
}

__MIDDLEWARE_END__
