/*
 * Copyright (c) 2020 Yuriy Lisovskiy
 */

#include "./settings.h"

#include <xalwart/core/path.h>
#include <xalwart/render/env/default.h>

#include "../picture_app/app.h"
#include "../form_app/app.h"
#include "../main_app/app.h"

#include "./libs/my_lib.h"

#include "./middleware/useful_middleware.h"

#include "./env/my_engine.h"


Settings::Settings() : xw::conf::Settings(
	xw::core::path::dirname(xw::core::path::dirname(__FILE__))
)
{
}

void Settings::register_apps()
{
	this->app<MainAppConfig>("MainApp");
	this->app<FormAppConfig>("FormApp");
	this->app<PictureAppConfig>("PictureApp");
}

void Settings::register_middleware()
{
	this->middleware<UsefulMiddleware>("UsefulMiddleware");
}

void Settings::register_libraries()
{
	this->library<MyLib>("MyLib");
}

void Settings::register_templates_env(xw::render::env::Config* cfg)
{
	this->TEMPLATES_ENV = xw::render::env::DefaultEnvironment<MyEngine>::make(cfg);
}
