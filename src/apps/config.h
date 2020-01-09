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

#pragma once

// C++ libraries.
#include <string>
#include <vector>

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "./interface.h"
#include "../views/view.h"
#include "../urls/url.h"
#include "../urls/pattern.h"
#include "../conf/settings.h"
#include "../utility/string/str.h"
#include "../core/management/base.h"
#include "../core/management/commands/app_command.h"


__APPS_BEGIN__

/// Derived class must contain constructor with
/// pointer to conf::Settings parameter.
class AppConfig : public IAppConfig
{
private:
	std::vector<urls::UrlPattern> _urlpatterns;
	std::vector<core::BaseCommand*> _commands;

	template <typename _AppConfigT>
	IAppConfig* find_or_create_app()
	{
		auto app = std::find_if(
			this->settings->INSTALLED_APPS.begin(),
			this->settings->INSTALLED_APPS.end(),
			[](IAppConfig* entry) -> bool {
				return dynamic_cast<_AppConfigT*>(entry) != nullptr;
			}
		);
		if (app == this->settings->INSTALLED_APPS.end())
		{
			return new _AppConfigT(this->settings);
		}

		return *app;
	}

protected:
	std::string app_name;
	conf::Settings* settings;

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

	template <typename _AppConfigT, typename = std::enable_if<std::is_base_of<IAppConfig, _AppConfigT>::value>>
	void include(const std::string& prefix)
	{
		auto app = this->find_or_create_app<_AppConfigT>();
		auto included_urlpatterns = app->get_urlpatterns();
		for (const auto& pattern : included_urlpatterns)
		{
			this->_urlpatterns.emplace_back(
				str::rtrim(str::starts_with(prefix, "/") ? prefix : "/" + prefix, '/'),
				pattern
			);
		}
	}

	template <typename _CommandT, typename = std::enable_if<std::is_base_of<core::cmd::AppCommand, _CommandT>::value>>
	void command()
	{
		auto* cmd = new _CommandT(this, this->settings);
		this->_commands.push_back(cmd);
	}

	explicit AppConfig(conf::Settings* settings);

public:
	std::string get_name() final;
	std::vector<urls::UrlPattern> get_urlpatterns() final;
	std::vector<core::BaseCommand*> get_commands() final;
	virtual void urlpatterns();
	virtual void commands();
};

__APPS_END__
