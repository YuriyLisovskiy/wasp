/*
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
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
 * apps/config.h
 *
 * Purpose:
 * 	Represents application configuration
 * 	with urls and models (will be developed in future)
 */

#pragma once

// Module definitions.
#include "./_def_.h"

// Framework modules.
#include "../views/view.h"
#include "../urls/url.h"
#include "../core/management/app_command.h"
#include "../core/string.h"


__APPS_BEGIN__

/// Derived class must contain constructor with
/// pointer to conf::Settings parameter.
class AppConfig : public IAppConfig, public core::object::Object
{
private:
	std::vector<std::shared_ptr<urls::UrlPattern>> _urlpatterns;
	std::vector<std::shared_ptr<core::BaseCommand>> _commands;

	template <typename AppConfigT>
	std::shared_ptr<apps::IAppConfig> find_or_create_app()
	{
		auto app = std::find_if(
			this->settings->INSTALLED_APPS.begin(),
			this->settings->INSTALLED_APPS.end(),
			[](const std::shared_ptr<apps::IAppConfig>& entry) -> bool {
				return dynamic_cast<AppConfigT*>(entry.get()) != nullptr;
			}
		);
		if (app == this->settings->INSTALLED_APPS.end())
		{
			return std::make_shared<AppConfigT>(this->settings);
		}

		return *app;
	}

protected:
	std::string app_name;
	std::string app_path;
	conf::Settings* settings;

	template <typename ViewT, typename = std::enable_if<std::is_base_of<views::View, ViewT>::value>>
	void url(const std::string& pattern, const std::string& name = "")
	{
		views::ViewHandler view_handler = [this](
			http::HttpRequest* request,
			views::Args* args,
			conf::Settings* settings_ptr
		) -> std::unique_ptr<http::IHttpResponse>
		{
			ViewT view(settings_ptr);
			view.setup(request);
			return view.dispatch(args);
		};

		this->_urlpatterns.push_back(urls::make_url(
			core::str::starts_with(pattern, "/") ? pattern : "/" + pattern,
			view_handler,
			name
		));
	}

	template <typename AppConfigT, typename = std::enable_if<std::is_base_of<IAppConfig, AppConfigT>::value>>
	void include(const std::string& prefix, const std::string& namespace_ = "")
	{
		auto app = this->find_or_create_app<AppConfigT>();
		auto included_urlpatterns = app->get_urlpatterns();
		std::string ns = namespace_.empty() ? app->get_name() : namespace_;
		for (const auto& pattern : included_urlpatterns)
		{
			this->_urlpatterns.push_back(std::make_shared<urls::UrlPattern>(
				core::str::rtrim(
					core::str::starts_with(prefix, "/") ? prefix : "/" + prefix,
					"/"
				),
				pattern,
				ns
			));
		}
	}

	template <typename CommandT, typename = std::enable_if<std::is_base_of<core::AppCommand, CommandT>::value>>
	void command()
	{
		auto cmd = std::make_shared<CommandT>(this, this->settings);
		this->_commands.push_back(cmd);
	}

	explicit AppConfig(
		const std::string& app_path, conf::Settings* settings
	);
	void init(const core::object::Type& type);

public:
	std::string get_name() final;
	std::string get_app_path() final;
	std::vector<std::shared_ptr<urls::UrlPattern>> get_urlpatterns() final;
	std::vector<std::shared_ptr<core::BaseCommand>> get_commands() final;
	virtual void urlpatterns();
	virtual void commands();
};

__APPS_END__
