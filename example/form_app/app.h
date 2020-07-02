/*
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 */

#pragma once

#include <xalwart/apps/config.h>
#include <xalwart/views/view.h>
#include <xalwart/conf/settings.h>

#include "./form_view.h"


class FormAppConfig : public xw::apps::AppConfig
{
public:
	explicit FormAppConfig(xw::conf::Settings* settings)
		: AppConfig(__FILE__, settings)
	{
	}

	void urlpatterns() override
	{
		this->url<FormView>(
			R"(profile/<user_id>([0-9]*)/name/<user_name>([A-Za-z]+)/?)",
			"profile"
		);
	}
};
