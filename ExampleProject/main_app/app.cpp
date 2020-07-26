/*
 * Copyright (c) 2020 Yura Lisovskiy
 */

#include "./app.h"

#include "./views/main_view.h"
#include "./views/redirect_view.h"

#include "../form_app/app.h"
#include "../picture_app/app.h"

#include "./commands/hello.h"


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
