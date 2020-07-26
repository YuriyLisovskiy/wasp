/*
 * Copyright (c) 2020 Yura Lisovskiy
 */

#include "./app.h"

#include "./views/picture_view.h"


PictureAppConfig::PictureAppConfig(xw::conf::Settings* settings)
	: AppConfig(__FILE__, settings)
{
}

void PictureAppConfig::urlpatterns()
{
	this->url<PictureView>(R"(view/pic/?)", "view_pic");
}
