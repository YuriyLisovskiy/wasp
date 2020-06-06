/*
 * Copyright (c) 2020 Yuriy Lisovskiy
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
 * conf/settings_factory.h
 *
 * Purpose:
 *  TODO: implement docs for conf/settings_factory.h
 */

#pragma once

// C++ libraries.
#include <memory>
#include <functional>

// Module definitions.
#include "./_def_.h"

// Framework modules.
#include "../core/logger.h"
#include "../apps/_def_.h"
#include "../middleware/_def_.h"
#include "../render/library/_def_.h"


__CONF_BEGIN__
struct Settings;
__CONF_END__

__APPS_BEGIN__
class IAppConfig;
__APPS_END__

__LIB_BEGIN__
class ILibrary;
__LIB_END__

__MIDDLEWARE_BEGIN__
class IMiddleware;
__MIDDLEWARE_END__


__CONF_INTERNAL_BEGIN__

class SettingsFactory final
{
private:
	std::map<
		std::string,
		std::function<std::shared_ptr<middleware::IMiddleware>()>
	> _middleware;
	std::map<
		std::string,
		std::function<std::shared_ptr<render::lib::ILibrary>()>
	> _libraries;

	std::map<
		std::string,
		std::function<std::shared_ptr<apps::IAppConfig>()>
	> _apps;

	Settings* _settings;
	core::ILogger* _logger;

public:
	SettingsFactory(nullptr_t) = delete;
	SettingsFactory(Settings* settings, core::ILogger* logger);

	template <typename T, typename = std::enable_if<std::is_base_of<apps::IAppConfig, T>::value>>
	void register_app(const std::string& full_name)
	{
		if (this->_apps.find(full_name) != this->_apps.end())
		{
			if (this->_logger)
			{
				this->_logger->warning(
					"unable to register '" + full_name + "' app which already exists"
				);
			}
		}
		else
		{
			this->_apps[full_name] = [this]() -> std::shared_ptr<apps::IAppConfig> {
				return std::make_shared<T>(this->_settings);
			};
		}
	}

	template <typename T, typename = std::enable_if<std::is_base_of<middleware::IMiddleware, T>::value>>
	void register_middleware(const std::string& full_name)
	{
		if (this->_middleware.find(full_name) != this->_middleware.end())
		{
			if (this->_logger)
			{
				this->_logger->warning(
					"unable to register '" + full_name + "' middleware which already exists"
				);
			}
		}
		else
		{
			this->_middleware[full_name] = [this]() -> std::shared_ptr<middleware::IMiddleware> {
				return std::make_shared<T>(this->_settings);
			};
		}
	}

	template <typename T, typename = std::enable_if<std::is_base_of<render::lib::ILibrary, T>::value>>
	void register_library(const std::string& full_name)
	{
		if (this->_libraries.find(full_name) != this->_libraries.end())
		{
			if (this->_logger)
			{
				this->_logger->warning(
					"unable to register '" + full_name + "' library which already exists"
				);
			}
		}
		else
		{
			this->_libraries[full_name] = [this]() -> std::shared_ptr<render::lib::ILibrary> {
				return std::make_shared<T>(this->_settings);
			};
		}
	}

	std::shared_ptr<apps::IAppConfig> get_app(const std::string& full_name);
	std::shared_ptr<middleware::IMiddleware> get_middleware(const std::string& full_name);
	std::shared_ptr<render::lib::ILibrary> get_library(const std::string& full_name);
};

__CONF_INTERNAL_END__
