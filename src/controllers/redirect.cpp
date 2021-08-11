/**
 * controllers/redirect.cpp
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 */

#include "./redirect.h"


__CONTROLLERS_BEGIN__

std::string RedirectController::get_redirect_url()
{
	std::string url = this->_url;
	if (this->_url.empty())
	{
		return url;
	}

	if (this->_query_string)
	{
		url += "?" + this->request->raw_query();
	}

	return url;
}

http::Response::Result RedirectController::get()
{
	std::string url = this->get_redirect_url();
	if (url.empty())
	{
		if (this->settings->LOGGER != nullptr)
		{
			this->settings->LOGGER->warning("Gone: " + request->path());
		}

		return http::result<http::resp::Gone>("");
	}

	if (this->_permanent)
	{
		return http::result<http::resp::PermanentRedirect>(url);
	}

	return http::result<http::resp::Redirect>(url);
}

__CONTROLLERS_END__
