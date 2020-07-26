/*
 * Copyright (c) 2020 Yura Lisovskiy
 */

#pragma once

#include <xalwart/apps/config.h>


class PictureAppConfig : public xw::apps::AppConfig
{
public:
	explicit PictureAppConfig(xw::conf::Settings* settings);
	void urlpatterns() override;
};
