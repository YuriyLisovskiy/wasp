/**
 * controllers/redirect.cpp
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 */

#include "./redirect.h"

// Framework libraries.
#include "../conf/settings.h"


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
		url += "?" + this->request->query();
	}

	return url;
}

Result<std::shared_ptr<http::IHttpResponse>> RedirectController::get(Kwargs* kwargs)
{
	std::string url = this->get_redirect_url();
	if (url.empty())
	{
		if (this->settings->LOGGER != nullptr)
		{
			this->settings->LOGGER->warning("Gone: " + request->path());
		}

		return this->response<http::HttpResponseGone>("");
	}

	if (this->_permanent)
	{
		return this->response<http::HttpResponsePermanentRedirect>(url);
	}

	return this->response<http::HttpResponseRedirect>(url);
}

__CONTROLLERS_END__
