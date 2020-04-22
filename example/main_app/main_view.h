/*
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 */

#pragma once

#include <string>

#include "../../src/views/template_view.h"
#include "../../src/views/redirect_view.h"
#include "../../src/core/types/array.h"


class MainView : public wasp::views::TemplateView
{
public:
	explicit MainView(wasp::conf::Settings* settings)
		: TemplateView({"get"}, settings)
	{
		this->_template_name = "main_app/index.html";
	}

	std::shared_ptr<wasp::render::IContext> get_context(
		wasp::http::HttpRequest* request, wasp::views::Args* args
	) override
	{
		using namespace wasp::render;
		using namespace wasp::core;

		auto test_array = types::Array();
		test_array.push_back("Yuriy Lisovskiy");
		test_array.push_back("John Doe");
		return std::make_shared<Context>(Context::scope_t{{
			"test_array", std::make_shared<types::Array>(test_array)
		}});
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
