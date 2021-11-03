/**
 * controllers/redirect.cpp
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 */

#include "./redirect.h"


__CONTROLLERS_BEGIN__

std::string RedirectController::get_redirect_url(http::Request* request) const
{
	std::string url = this->_url;
	if (this->_url.empty())
	{
		return url;
	}

	if (this->_query_string)
	{
		url += "?" + request->url().raw_query;
	}

	return url;
}

std::unique_ptr<http::HttpResponse> RedirectController::get(http::Request* request) const
{
	std::string url = this->get_redirect_url(request);
	if (url.empty())
	{
		if (this->logger)
		{
			this->logger->warning("Gone: " + request->url().path);
		}

		return std::make_unique<http::Gone>("");
	}

	if (this->_permanent)
	{
		return std::make_unique<http::PermanentRedirect>(url);
	}

	return std::make_unique<http::Redirect>(url);
}

__CONTROLLERS_END__
