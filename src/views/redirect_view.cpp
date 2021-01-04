/**
 * views/redirect_view.cpp
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 */

#include "./redirect_view.h"


__VIEWS_BEGIN__

RedirectView::RedirectView(
	conf::Settings* settings,
	const std::string& url,
	bool permanent,
	bool query_string
)
	: View(
		{"get", "post", "put", "patch", "delete", "head", "options"},
		settings
	)
{
	this->_url = url;
	this->_permanent = permanent;
	this->_query_string = query_string;
}

std::string RedirectView::get_redirect_url()
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

core::Result<std::shared_ptr<http::IHttpResponse>> RedirectView::get(http::HttpRequest* request, Args* args)
{
	std::string url = this->get_redirect_url();
	if (url.empty())
	{
		if (this->logger != nullptr)
		{
			this->logger->warning("Gone: " + request->path());
		}

		return this->response<http::HttpResponseGone>("");
	}

	if (this->_permanent)
	{
		return this->response<http::HttpResponsePermanentRedirect>(url);
	}

	return this->response<http::HttpResponseRedirect>(url);
}

core::Result<std::shared_ptr<http::IHttpResponse>> RedirectView::post(http::HttpRequest* request, Args* args)
{
	return this->get(request, args);
}

core::Result<std::shared_ptr<http::IHttpResponse>> RedirectView::put(http::HttpRequest* request, Args* args)
{
	return this->get(request, args);
}

core::Result<std::shared_ptr<http::IHttpResponse>> RedirectView::patch(http::HttpRequest* request, Args* args)
{
	return this->get(request, args);
}

core::Result<std::shared_ptr<http::IHttpResponse>> RedirectView::delete_(http::HttpRequest* request, Args* args)
{
	return this->get(request, args);
}

core::Result<std::shared_ptr<http::IHttpResponse>> RedirectView::head(http::HttpRequest* request, Args* args)
{
	return this->get(request, args);
}

core::Result<std::shared_ptr<http::IHttpResponse>> RedirectView::options(http::HttpRequest* request, Args* args)
{
	return this->get(request, args);
}

__VIEWS_END__
