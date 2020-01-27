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
 * An implementation of cache.h.
 */

#include "./cache.h"


__CACHE_INTERNAL_BEGIN__

// TESTME: test _if_match_passes
bool _if_match_passes(
	const std::string& target_etag, std::vector<std::string>& etags
)
{
	// TODO: implement _if_match_passes
	return false;
}

// TESTME: test _if_none_match_passes
bool _if_none_match_passes(
	const std::string& target_etag, std::vector<std::string>& etags
)
{
	// TODO: implement _if_none_match_passes
	return false;
}

// TESTME: test _if_modified_since_passes
bool _if_modified_since_passes(
	long last_modified, long if_modified_since
)
{
	return last_modified == -1 || last_modified > if_modified_since;
}

// TESTME: test _if_unmodified_since_passes
bool _if_unmodified_since_passes(
	long last_modified, long if_unmodified_since
)
{
	return last_modified != -1 && last_modified <= if_unmodified_since;
}

// TESTME: test _precondition_failed
http::HttpResponseBase* _precondition_failed(http::HttpRequest* request)
{
	// TODO: implement _precondition_failed
	return nullptr;
}

// TESTME: test _not_modified
http::HttpResponseBase* _not_modified(
	http::HttpRequest* request, http::HttpResponseBase* response
)
{
	// TODO: implement _not_modified
	return nullptr;
}

__CACHE_INTERNAL_END__


__CACHE_BEGIN__

// TESTME: test set_response_etag
void set_response_etag(http::HttpResponseBase* response)
{
	if (!response->is_streaming() && response->content_length() > 0)
	{
		response->set_header(
			http::E_TAG,
			utils_http::quote_etag(crypto::md5(response->get_content()).digest())
		);
	}
}

// TESTME: test get_conditional_response
http::HttpResponseBase* get_conditional_response(
	http::HttpRequest* request,
	const std::string& etag,
	long last_modified,
	http::HttpResponseBase* response
)
{
	// Only return conditional responses on successful requests.
	if (response && !(response->status() >= 200 && response->status() < 300))
	{
		return nullptr;
	}

	// Get HTTP request headers.
	auto if_match_etags = utils_http::parse_etags(
		request->headers.get(http::IF_MATCH, "")
	);
	long if_unmodified_since = utils_http::parse_http_date(
		request->headers.get(http::IF_UNMODIFIED_SINCE, "")
	);
	auto if_none_match_etags = utils_http::parse_etags(
		request->headers.get(http::IF_NONE_MATCH, "")
	);
	long if_modified_since = utils_http::parse_http_date(
		request->headers.get(http::IF_MODIFIED_SINCE)
	);

	// Step 1 of section 6 of RFC 7232: Test the If-Match precondition.
	if (!if_match_etags.empty() && !internal::_if_match_passes(etag, if_match_etags))
	{
		return internal::_precondition_failed(request);
	}

	// Step 2: Test the If-Unmodified-Since precondition.
	if (
		if_match_etags.empty() &&
		if_unmodified_since != -1 &&
		!internal::_if_unmodified_since_passes(last_modified, if_unmodified_since)
	)
	{
		return internal::_precondition_failed(request);
	}

	// Step 3: Test the If-None-Match precondition.
	if (!if_none_match_etags.empty() && !internal::_if_none_match_passes(etag, if_none_match_etags))
	{
		if (request->method() == "GET" || request->method() == "HEAD")
		{
			return internal::_not_modified(request, response);
		}
		else
		{
			return internal::_precondition_failed(request);
		}
	}

	// Step 4: Test the If-Modified-Since precondition.
	if (
		if_none_match_etags.empty() &&
		if_modified_since != -1 &&
		!internal::_if_modified_since_passes(last_modified, if_modified_since)
	)
	{
		if (request->method() == "GET" || request->method() == "HEAD")
		{
			return internal::_not_modified(request, response);
		}
	}

	// Step 5: Test the If-Range precondition (not supported).
	// Step 6: Return nullptr since there isn't a conditional response.
	return nullptr;
}

__CACHE_END__
