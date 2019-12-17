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
 * An implementation of redirect_view.h.
 */

#include "redirect_view.h"


__VIEWS_BEGIN__

RedirectView::RedirectView(const std::string& url, bool permanent, bool query_string, ILogger* logger)
	: View({"get", "post", "put", "patch", "delete", "head", "options"}, logger)
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
		url += "?" + this->_request->query();
	}

	return url;
}

wasp::http::HttpResponse* RedirectView::get(wasp::http::HttpRequest* request, Args* args)
{
	std::string url = this->get_redirect_url();
	if (url.empty())
	{
		if (this->_logger != nullptr)
		{
			this->_logger->warning("Gone: " + request->path(), _DETAILS_NONE_);
		}

		return new wasp::http::HttpResponseGone("");
	}

	if (this->_permanent)
	{
		return new wasp::http::HttpResponsePermanentRedirect(url);
	}

	return new wasp::http::HttpResponseRedirect(url);
}

wasp::http::HttpResponse* RedirectView::post(wasp::http::HttpRequest* request, Args* args)
{
	return this->get(request, args);
}

wasp::http::HttpResponse* RedirectView::put(wasp::http::HttpRequest* request, Args* args)
{
	return this->get(request, args);
}

wasp::http::HttpResponse* RedirectView::patch(wasp::http::HttpRequest* request, Args* args)
{
	return this->get(request, args);
}

wasp::http::HttpResponse* RedirectView::delete_(wasp::http::HttpRequest* request, Args* args)
{
	return this->get(request, args);
}

wasp::http::HttpResponse* RedirectView::head(wasp::http::HttpRequest* request, Args* args)
{
	return this->get(request, args);
}

wasp::http::HttpResponse* RedirectView::options(wasp::http::HttpRequest* request, Args* args)
{
	return this->get(request, args);
}

__VIEWS_END__
