/*
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 */

#pragma once

#include "../../src/apps/config.h"
#include "../../src/views/view.h"
#include "../../src/conf/settings.h"

#include "./form_view.h"


class FormAppConfig : public xw::apps::AppConfig
{
public:
	explicit FormAppConfig(xw::conf::Settings* settings)
		: AppConfig(__FILE__, settings)
	{
		this->init(this->__type__());
	}

	void urlpatterns() override
	{
		this->url<FormView>(
			R"(profile/<user_id>([0-9]*)/name/<user_name>([A-Za-z]+)/?)",
			"profile"
		);
	}
};
