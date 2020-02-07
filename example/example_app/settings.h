/*
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 */

#pragma once

#include "../../src/conf/settings.h"
#include "../../src/core/path.h"

#include "../../src/middleware/common.h"
#include "../../src/middleware/cookies.h"
#include "../../src/middleware/security.h"
#include "../../src/middleware/clickjacking.h"

#include "../../src/core/management/commands/runserver.h"

#include "../picture_app/app.h"
#include "../form_app/app.h"
#include "../main_app/app.h"


struct Settings final: public wasp::conf::Settings
{
	Settings() : wasp::conf::Settings()
	{
	}

	void init() final
	{
		this->BASE_DIR = wasp::core::path::dirname(wasp::core::path::dirname(__FILE__));

		this->SECRET_KEY = "+s6cv712&nw4gsk)1dmgpje+f#%^4lhp@!up+=p3ts+hxz(fr2";

		this->DEBUG = true;

		this->ALLOWED_HOSTS = {"127.0.0.1"};

		this->INSTALLED_APPS = {
			this->app<MainAppConfig>(),
			this->app<FormAppConfig>(),
			this->app<PictureAppConfig>(),
		};

		this->MIDDLEWARE = {
			this->middleware<wasp::middleware::SecurityMiddleware>(),
			this->middleware<wasp::middleware::CommonMiddleware>(),
			this->middleware<wasp::middleware::XFrameOptionsMiddleware>(),
			this->middleware<wasp::middleware::CookieMiddleware>()
		};

		this->MEDIA_ROOT = wasp::core::path::join(this->BASE_DIR, "media");
		this->MEDIA_URL = "/media/";

		this->STATIC_ROOT = wasp::core::path::join(this->BASE_DIR, "static");
		this->STATIC_URL = "/static/";

		this->DATA_UPLOAD_MAX_MEMORY_SIZE = 20971520;
	}

	// Override in local_settings.cpp!
	void overwrite() final;
};
