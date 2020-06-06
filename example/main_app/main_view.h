/*
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 */

#pragma once

#include <string>

#include <xalwart/views/template_view.h>
#include <xalwart/views/redirect_view.h>
#include <xalwart/core/types/array.h>


class MainView : public xw::views::TemplateView
{
public:
	explicit MainView(xw::conf::Settings* settings)
		: TemplateView({"get"}, settings)
	{
		this->_template_name = "main_app/index.html";
	}

	std::shared_ptr<xw::render::IContext> get_context(
		xw::http::HttpRequest* request, xw::views::Args* args
	) override
	{
		using namespace xw::render;
		using namespace xw::core;

		auto test_array = types::Array();
		test_array.push_back("Yuriy Lisovskiy");
		test_array.push_back("John Doe");
		return std::make_shared<Context>(Context::scope_t{{
			"test_array", std::make_shared<types::Array>(test_array)
		}});
	}
};


class RedirectView : public xw::views::RedirectView
{
public:
	explicit RedirectView(xw::conf::Settings* settings)
		: xw::views::RedirectView(settings, "/index", false, false)
	{
	};
};
