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

#include "../../src/apps/config.h"
#include "../../src/views/view.h"
#include "../../src/conf/settings.h"

#include "./main_view.h"
#include "../form_app/app.h"
#include "../picture_app/app.h"


class MainAppConfig : public wasp::apps::AppConfig
{
public:
	explicit MainAppConfig(wasp::conf::Settings* settings) : AppConfig(settings)
	{
	}

	void urlpatterns() override
	{
		this->url<MainView>(R"(index/?)", "index");
		this->include<PictureAppConfig>(R"(picture/)");
		this->include<FormAppConfig>(R"(form/)");
		this->url<RedirectView>(R"(/?)", "root");
	}
};
