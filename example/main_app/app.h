/*
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 */

#pragma once

#include <xalwart/apps/config.h>


class MainAppConfig : public xw::apps::AppConfig
{
public:
	explicit MainAppConfig(xw::conf::Settings* settings);
	void urlpatterns() override;
	void commands() override;
};
