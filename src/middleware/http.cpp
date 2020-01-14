/*
 * Copyright (c) 2019 Yuriy Lisovskiy
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
 * An implementation of http.h.
 */

#include "./http.h"


__MIDDLEWARE_BEGIN__

ConditionalGetMiddleware::ConditionalGetMiddleware(
	conf::Settings* settings
) : MiddlewareMixin(settings)
{
}

http::HttpResponseBase* ConditionalGetMiddleware::process_response(
	http::HttpRequest* request, http::HttpResponseBase* response
)
{
	/// It's too late to prevent an unsafe request with a 412 response, and
	/// for a HEAD request, the response body is always empty so computing
	/// an accurate ETag isn't possible.
	if (request->method() != "GET")
	{
		return nullptr;
	}

	if (needs_etag(response) && !response->has_header(http::E_TAG))
	{
		utils::cache::set_response_etag(response);
	}

	auto etag = response->get_header(http::E_TAG, "");
	auto last_modified = response->get_header(http::LAST_MODIFIED, "");
	// TODO: parse date safe;
	//  write it to last_modified (create new variable).

	if (!etag.empty() || !last_modified.empty())
	{
		auto conditional_response = utils::cache::get_conditional_response(
			request, etag, last_modified, response
		);
		if (conditional_response)
		{
			return conditional_response;
		}
	}

	return nullptr;
}

bool ConditionalGetMiddleware::needs_etag(
	http::HttpResponseBase* response
)
{
	auto cache_control = core::str::split(
		response->get_header(http::CACHE_CONTROL, ""), ','
	);
	bool result = true;
	for (const auto& directive : cache_control)
	{
		if (core::str::lower(core::str::trim(directive)) == "no-store")
		{
			result = false;
			break;
		}
	}

	return result;
}

__MIDDLEWARE_END__
