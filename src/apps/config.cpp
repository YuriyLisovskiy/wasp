/**
 * apps/config.cpp
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 */

#include "./config.h"


__APPS_BEGIN__

AppConfig::AppConfig(const std::string& app_path, conf::Settings* settings)
{
	this->_is_initialized = false;
	this->settings = settings;
	this->app_path = app_path;
}

void AppConfig::init(const object::Type& type)
{
	if (!this->_is_initialized)
	{
		std::string name = type.name();
		str::rtrim(name, "Config");
		str::rtrim(name, "config");
		str::rtrim(name, "_");
		str::rtrim(name, "App");
		str::rtrim(name, "app");
		str::rtrim(name, "_");
		this->app_name = name;
		this->_is_initialized = true;
	}
}

bool AppConfig::is_initialized() const
{
	return this->_is_initialized;
}

std::string AppConfig::get_name()
{
	return this->app_name;
}

std::string AppConfig::get_app_path()
{
	return this->app_path;
}

std::vector<std::shared_ptr<urls::UrlPattern>> AppConfig::get_urlpatterns()
{
	if (this->_urlpatterns.empty())
	{
		this->urlpatterns();
	}

	return this->_urlpatterns;
}

std::vector<std::shared_ptr<cmd::BaseCommand>> AppConfig::get_commands()
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
