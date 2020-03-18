/*
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 */

#pragma once

#include <string>

#include "../../src/views/view.h"


class PictureView : public wasp::views::View
{
public:
	explicit PictureView(wasp::conf::Settings* settings)
		: View({"get"}, settings)
	{
	}

	std::unique_ptr<wasp::http::IHttpResponse> get(wasp::http::HttpRequest* request, wasp::views::Args* args) final
	{
		std::string body(R"(<img src="/static/pontar.png" alt="Night over Pontar river" width="1000">)");
		return std::make_unique<wasp::http::HttpResponse>(body);
	}
};
