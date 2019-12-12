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

#ifndef WASP_VIEWS_REDIRECT_VIEW_H
#define WASP_VIEWS_REDIRECT_VIEW_H

#include <string>

#include "../globals.h"
#include "view.h"
#include "args.h"
#include "../utility/logger.h"
#include "../http/request.h"
#include "../http/response.h"


__WASP_BEGIN__

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
		ILogger* logger = nullptr
	);

	/// Return the URL redirect to.
	std::string get_redirect_url();

	HttpResponse* get(HttpRequest* request, Args* args) override;
	HttpResponse* post(HttpRequest* request, Args* args) override;
	HttpResponse* head(HttpRequest* request, Args* args) override;
	HttpResponse* options(HttpRequest* request, Args* args) override;
	HttpResponse* put(HttpRequest* request, Args* args) override;
	HttpResponse* delete_(HttpRequest* request, Args* args) override;
	HttpResponse* patch(HttpRequest* request, Args* args) override;
};

__WASP_END__


#endif // WASP_VIEWS_REDIRECT_VIEW_H
