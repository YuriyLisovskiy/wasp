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

#ifndef WASP_EXAMPLE_EXAMPLE_APP_CONFIG_H
#define WASP_EXAMPLE_EXAMPLE_APP_CONFIG_H

#include "../../src/apps/config.h"
#include "../../src/views/view.h"

#include "./main_view.h"
#include "../form_app/config.h"
#include "../picture_app/config.h"


class MainAppConfig : public wasp::apps::AppConfig
{
public:
	MainAppConfig()
	{
		this->url(
			R"(index/?)",
			wasp::views::View::make_view<MainView>(),
			"index"
		);
		this->include(R"(picture/)", picture_app_config);
		this->include(R"(form/)", form_app_config);
		this->url(
			R"(/?)",
			wasp::views::View::make_view<RedirectView>(),
			"root"
		);
	}
};

MainAppConfig* main_app_config = new MainAppConfig();


#endif // WASP_EXAMPLE_EXAMPLE_APP_CONFIG_H
