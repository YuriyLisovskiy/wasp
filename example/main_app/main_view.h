/*
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 */

#pragma once

#include <string>

#include "../../src/views/template_view.h"
#include "../../src/views/redirect_view.h"


class MainView : public wasp::views::TemplateView
{
public:
	explicit MainView(wasp::conf::Settings* settings)
		: TemplateView({"get"}, settings)
	{
		this->_template_name = "main_app/index.html";
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
