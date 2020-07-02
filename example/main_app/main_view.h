/*
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 */

#pragma once

#include <string>

#include <xalwart/views/template_view.h>
#include <xalwart/views/redirect_view.h>


class MainView : public xw::views::TemplateView
{
public:
	explicit MainView(xw::conf::Settings* settings);

	std::shared_ptr<xw::render::IContext> get_context(
		xw::http::HttpRequest* request, xw::views::Args* args
	) override;
};


class RedirectView : public xw::views::RedirectView
{
public:
	explicit RedirectView(xw::conf::Settings* settings);
};
