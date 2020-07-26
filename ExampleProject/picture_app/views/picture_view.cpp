/*
 * Copyright (c) 2020 Yura Lisovskiy
 */

#include "./picture_view.h"


PictureView::PictureView(xw::conf::Settings* settings)
	: TemplateView({"get"}, settings)
{
	this->_template_name = "picture/river.html";
}
