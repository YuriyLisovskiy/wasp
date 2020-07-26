/*
 * Copyright (c) 2020 Yura Lisovskiy
 */

#pragma once

#include <xalwart/views/template_view.h>


class FormView : public xw::views::TemplateView
{
public:
	explicit FormView(xw::conf::Settings* settings);

	std::unique_ptr<xw::http::IHttpResponse> get(
		xw::http::HttpRequest* request, xw::views::Args* args
	) final;
	std::unique_ptr<xw::http::IHttpResponse> post(
		xw::http::HttpRequest* request, xw::views::Args* args
	) final;
};
