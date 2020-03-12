/*
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 */

#pragma once

#include "../../src/apps/config.h"
#include "../../src/conf/settings.h"

#include "./picture_view.h"


class PictureAppConfig : public wasp::apps::AppConfig
{
public:
	explicit PictureAppConfig(wasp::conf::Settings* settings)
		: AppConfig(__FILE__, settings)
	{
		this->init(this->__type__());
	}

	void urlpatterns() override
	{
		this->url<PictureView>(R"(view/pic/?)", "view_pic");
	}
};
