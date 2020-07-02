/*
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 */

#include "./app.h"

#include "./main_view.h"
#include "../form_app/app.h"
#include "../picture_app/app.h"
#include "./commands/hello_command.h"


MainAppConfig::MainAppConfig(xw::conf::Settings* settings)
	: AppConfig(__FILE__, settings)
{
}

void MainAppConfig::urlpatterns()
{
	this->url<MainView>(R"(index/?)", "index");
	this->include<PictureAppConfig>(R"(picture/)", "picture");
	this->include<FormAppConfig>(R"(form/)");
	this->url<RedirectView>(R"(/?)", "root");
}

void MainAppConfig::commands()
{
	this->command<HelloCommand>();
}
