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

/**
 * config.h
 * Purpose: represents application configuration
 * 			with urls and models (will be developed in future)
 */

#ifndef WASP_APPS_CONFIG_H
#define WASP_APPS_CONFIG_H

// C++ libraries.
#include <vector>

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "../views/view.h"
#include "../urls/url.h"
#include "../urls/pattern.h"
#include "../utility/string/str.h"


__APPS_BEGIN__

class AppConfig
{
private:
	std::vector<urls::UrlPattern> _urlpatterns;

protected:
	template <typename _ViewT, typename = std::enable_if<std::is_base_of<views::View, _ViewT>::value>>
	void url(const std::string& pattern, const std::string& name = "")
	{
		views::ViewHandler view_handler = [](
			http::HttpRequest* request,
			views::Args* args,
			utility::ILogger* logger
		) -> http::HttpResponseBase*
		{
			_ViewT view(logger);
			view.setup(request);
			return view.dispatch(args);
		};

		this->_urlpatterns.push_back(urls::make_url(
			str::starts_with(pattern, "/") ? pattern : "/" + pattern,
			view_handler,
			name
		));
	}

	void include(const std::string& prefix, AppConfig* app);

public:
	AppConfig() = default;
	std::vector<urls::UrlPattern> get_urlpatterns();
};

__APPS_END__


#endif // WASP_APPS_CONFIG_H
