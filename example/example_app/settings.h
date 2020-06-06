/*
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 */

#pragma once

#include "../../src/conf/settings.h"
#include "../../src/core/path.h"

#include "../picture_app/app.h"
#include "../form_app/app.h"
#include "../main_app/app.h"

#include "./libs/my_first_lib.h"


struct Settings final: public xw::conf::Settings
{
	Settings() : xw::conf::Settings(
		xw::core::path::dirname(xw::core::path::dirname(__FILE__))
	)
	{
	}

	void register_apps() override
	{
		this->app<MainAppConfig>("MainApp");
		this->app<FormAppConfig>("FormApp");
		this->app<PictureAppConfig>("PictureApp");
	}

	void register_libraries() override
	{
		this->library<MyFirstLib>("MyFirstLib");
	}
};
