/*
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 */

#pragma once

#include <string>

#include "../../src/views/view.h"
#include "../../src/views/redirect_view.h"


class MainView : public wasp::views::View
{
public:
	explicit MainView(wasp::conf::Settings* settings)
		: View({"get"}, settings)
	{
	}

	wasp::http::HttpResponseBase* get(wasp::http::HttpRequest* request, wasp::views::Args* args) final
	{
		std::string body(
			"<h2>Hello from main view!</h2>"
			"<ul>"
			"   <li><a href=\"/form/profile/2048/name/YuriyLisovskiy\">Go to form!</a></li>"
			"   <li><a href=\"/picture/view/pic\">View picture!</a></li>"
			"</ul>"
		);
		return new wasp::http::HttpResponse(body);
	}
};


class RedirectView : public wasp::views::RedirectView
{
public:
	explicit RedirectView(wasp::conf::Settings* settings)
		: wasp::views::RedirectView(settings, "/index", false, false)
	{
	};
};
