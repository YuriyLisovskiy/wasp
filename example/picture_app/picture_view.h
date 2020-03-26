/*
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 */

#pragma once

#include <string>

#include "../../src/views/template_view.h"


class PictureView : public wasp::views::TemplateView
{
public:
	explicit PictureView(wasp::conf::Settings* settings)
		: TemplateView({"get"}, settings)
	{
		this->_template_name = "picture_app/river.html";
	}
};
