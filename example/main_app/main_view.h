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
#pragma once

#include <string>

#include "../../src/views/view.h"
#include "../../src/views/redirect_view.h"


class MainView : public wasp::views::View
{
public:
	explicit MainView(wasp::utility::ILogger* logger = nullptr)
		: View({"get"}, logger)
	{
	}

	wasp::http::HttpResponseBase* get(wasp::http::HttpRequest* request, wasp::views::Args* args) final
	{
		std::string body(
			"<h2>Hello from main view!</h2>"
			"<ul>"
			"   <li><a href=\"form/profile/2048/name/YuriyLisovskiy\">Go to form!</a></li>"
			"   <li><a href=\"picture/view/pic/\">View picture!</a></li>"
			"</ul>"
		);
		return new wasp::http::HttpResponse(body);
	}
};


class RedirectView : public wasp::views::RedirectView
{
public:
	explicit RedirectView(wasp::utility::ILogger* logger = nullptr)
		: wasp::views::RedirectView("/index", false, false, logger)
	{
	};
};
