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
 * views/redirect_view.h
 *
 * Purpose: provides a redirect on any GET request.
 */

#pragma once

// Module definitions.
#include "./_def_.h"

// Framework modules.
#include "./view.h"


__VIEWS_BEGIN__

class RedirectView : public View
{
private:
	std::string _url;
	bool _permanent;
	bool _query_string;

public:
	explicit RedirectView(
		conf::Settings* settings,
		const std::string& url,
		bool permanent = false,
		bool query_string = false
	);

	/// Return the URL redirect to.
	std::string get_redirect_url();

	std::unique_ptr<http::IHttpResponse> get(http::HttpRequest* request, Args* args) override;
	std::unique_ptr<http::IHttpResponse> post(http::HttpRequest* request, Args* args) override;
	std::unique_ptr<http::IHttpResponse> head(http::HttpRequest* request, Args* args) override;
	std::unique_ptr<http::IHttpResponse> options(http::HttpRequest* request, Args* args) override;
	std::unique_ptr<http::IHttpResponse> put(http::HttpRequest* request, Args* args) override;
	std::unique_ptr<http::IHttpResponse> delete_(http::HttpRequest* request, Args* args) override;
	std::unique_ptr<http::IHttpResponse> patch(http::HttpRequest* request, Args* args) override;
};

__VIEWS_END__
