/*
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 */

#pragma once

#include <xalwart/apps/config.h>
#include <xalwart/conf/settings.h>

#include "./picture_view.h"


class PictureAppConfig : public xw::apps::AppConfig
{
public:
	explicit PictureAppConfig(xw::conf::Settings* settings)
		: AppConfig(__FILE__, settings)
	{
		this->init(this->__type__());
	}

	void urlpatterns() override
	{
		this->url<PictureView>(R"(view/pic/?)", "view_pic");
	}
};
