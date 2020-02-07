/*
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 */

#pragma once

#include "../../src/apps/config.h"
#include "../../src/views/view.h"
#include "../../src/conf/settings.h"

#include "./main_view.h"
#include "../form_app/app.h"
#include "../picture_app/app.h"
#include "./commands/hello_command.h"


class MainAppConfig : public wasp::apps::AppConfig
{
public:
	explicit MainAppConfig(wasp::conf::Settings* settings)
		: AppConfig(__FILE__, settings)
	{
	}

	void urlpatterns() override
	{
		this->url<MainView>(R"(index/?)", "index");
		this->include<PictureAppConfig>(R"(picture/)");
		this->include<FormAppConfig>(R"(form/)");
		this->url<RedirectView>(R"(/?)", "root");
	}

	void commands() override
	{
		this->command<HelloCommand>();
	}
};
