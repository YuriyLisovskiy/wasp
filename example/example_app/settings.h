/*
 * Copyright (c) 2019 Yuriy Lisovskiy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include "../../src/conf/settings.h"
#include "../../src/utility/path.h"
#include "../../src/middleware/cookies.h"
#include "../../src/core/management/commands/runserver.h"

#include "../picture_app/app.h"
#include "../form_app/app.h"
#include "../main_app/app.h"

// #define DETECT_MEMORY_LEAK
#include "../../tests/mem_leak_check.h"


struct Settings final: public wasp::conf::Settings
{
	Settings() : wasp::conf::Settings()
	{
	}

	void init() final
	{
		this->BASE_DIR = wasp::path::dirname(wasp::path::dirname(__FILE__));

		this->SECRET_KEY = "+s6cv712&nw4gsk)1dmgpje+f#%^4lhp@!up+=p3ts+hxz(fr2";

		this->DEBUG = true;

		this->ALLOWED_HOSTS = {"127.0.0.1"};

		this->INSTALLED_APPS = {
			this->app<MainAppConfig>(),
			this->app<FormAppConfig>(),
			this->app<PictureAppConfig>(),
		};

		this->MIDDLEWARE = {
			this->middleware<wasp::middleware::CookieMiddleware>()
		};

		this->COMMANDS = {
			this->command<wasp::core::cmd::RunserverCommand>()
		};

		this->MEDIA_ROOT = wasp::path::join(this->BASE_DIR, "media");
		this->MEDIA_URL = "/media/";

		this->STATIC_ROOT = wasp::path::join(this->BASE_DIR, "static");
		this->STATIC_URL = "/static/";
	}

	// Override in local_settings.cpp!
	void overwrite() final;
};
