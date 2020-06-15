/*
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 */

#include "./main_view.h"

#include <xalwart/core/types/array.h>
#include <xalwart/render/context.h>


MainView::MainView(xw::conf::Settings* settings)
	: TemplateView({"get"}, settings)
{
	this->_template_name = "main_app/index.html";
}

std::shared_ptr<xw::render::IContext> MainView::get_context(
	xw::http::HttpRequest* request, xw::views::Args* args
)
{
	using Context = xw::render::Context;
	using Array = xw::core::types::Array;
	auto array = Array();
	array.push_back("Yuriy Lisovskiy");
	array.push_back("John Doe");
	return std::make_shared<Context>(Context::scope_t{{
		"my_array", std::make_shared<Array>(array)
	}});
}

RedirectView::RedirectView(xw::conf::Settings* settings)
	: xw::views::RedirectView(settings, "/index", false, false)
{
}
