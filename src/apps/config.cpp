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
 * An implementation of apps/config.h
 */

#include "./config.h"


__APPS_BEGIN__

AppConfig::AppConfig(const std::string& app_path, conf::Settings* settings)
{
	this->settings = settings;
	this->app_path = app_path;
	auto name = core::str::rtrim(this->__type__().name(), "AppConfig");
	core::str::rtrim(name, "appconfig");
	core::str::rtrim(name, "app_config");
	core::str::rtrim(name, "_");
	this->app_name = name;
}

std::string AppConfig::get_name()
{
	return this->app_name;
}

std::string AppConfig::get_app_path()
{
	return this->app_path;
}

std::vector<urls::UrlPattern> AppConfig::get_urlpatterns()
{
	if (this->_urlpatterns.empty())
	{
		this->urlpatterns();
	}

	return this->_urlpatterns;
}

std::vector<core::BaseCommand*> AppConfig::get_commands()
{
	if (this->_commands.empty())
	{
		this->commands();
	}

	return this->_commands;
}

void AppConfig::urlpatterns()
{
}

void AppConfig::commands()
{
}

__APPS_END__
