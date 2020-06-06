/*
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 */

#pragma once

#include <xalwart/apps/config.h>
#include <xalwart/views/view.h>
#include <xalwart/conf/settings.h>

#include "./main_view.h"
#include "../form_app/app.h"
#include "../picture_app/app.h"
#include "./commands/hello_command.h"


class MainAppConfig : public xw::apps::AppConfig
{
public:
	explicit MainAppConfig(xw::conf::Settings* settings)
		: AppConfig(__FILE__, settings)
	{
		this->init(this->__type__());
	}

	void urlpatterns() override
	{
		this->url<MainView>(R"(index/?)", "index");
		this->include<PictureAppConfig>(R"(picture/)", "picture");
		this->include<FormAppConfig>(R"(form/)");
		this->url<RedirectView>(R"(/?)", "root");
	}

	void commands() override
	{
		this->command<HelloCommand>();
	}
};
