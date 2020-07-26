/*
 * Copyright (c) 2020 Yura Lisovskiy
 */

#pragma once

#include <xalwart/views/template_view.h>


class MainView : public xw::views::TemplateView
{
public:
	explicit MainView(xw::conf::Settings* settings);

	std::shared_ptr<xw::render::IContext> get_context(
		xw::http::HttpRequest* request, xw::views::Args* args
	) override;
};
