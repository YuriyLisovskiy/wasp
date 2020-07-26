/*
 * Copyright (c) 2020 Yura Lisovskiy
 */

#include "./settings.h"

#include <xalwart/core/path.h>

#include "../main_app/app.h"
#include "../form_app/app.h"
#include "../picture_app/app.h"

#include "./middleware/useful.h"

#include "./libs/my_stuff/library.h"


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
	this->library<MyStuff>("MyStuff");
}
