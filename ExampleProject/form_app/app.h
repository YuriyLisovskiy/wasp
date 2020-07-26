/*
 * Copyright (c) 2020 Yura Lisovskiy
 */

#pragma once

#include <xalwart/apps/config.h>


class FormAppConfig : public xw::apps::AppConfig
{
public:
	explicit FormAppConfig(xw::conf::Settings* settings);
	void urlpatterns() override;
};
