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
 * redirect_view.h
 * Purpose: provide a redirect on any GET request.
 */

#pragma once

// C++ libraries.
#include <string>

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "./view.h"
#include "./args.h"
#include "../core/logger.h"
#include "../http/request.h"
#include "../http/response.h"


__VIEWS_BEGIN__

class RedirectView : public View
{
private:
	std::string _url;
	bool _permanent;
	bool _query_string;

public:
	explicit RedirectView(
		const std::string& url,
		bool permanent = false,
		bool query_string = false,
		core::ILogger* logger = nullptr
	);

	/// Return the URL redirect to.
	std::string get_redirect_url();

	http::HttpResponse* get(http::HttpRequest* request, Args* args) override;
	http::HttpResponse* post(http::HttpRequest* request, Args* args) override;
	http::HttpResponse* head(http::HttpRequest* request, Args* args) override;
	http::HttpResponse* options(http::HttpRequest* request, Args* args) override;
	http::HttpResponse* put(http::HttpRequest* request, Args* args) override;
	http::HttpResponse* delete_(http::HttpRequest* request, Args* args) override;
	http::HttpResponse* patch(http::HttpRequest* request, Args* args) override;
};

__VIEWS_END__
