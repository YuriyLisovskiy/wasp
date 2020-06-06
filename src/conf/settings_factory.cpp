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
 * An implementation of conf/settings_factory.h
 */


// Header.
#include "./settings_factory.h"

// Framework modules.
#include "../render/library/builtin.h"
#include "../middleware/clickjacking.h"
#include "../middleware/common.h"
#include "../middleware/cookies.h"
#include "../middleware/http.h"
#include "../middleware/security.h"


__CONF_INTERNAL_BEGIN__

SettingsFactory::SettingsFactory(Settings* settings)
{
	this->_settings = settings;

	using namespace middleware;
	this->_middleware = {
		{XFrameOptionsMiddleware::FULL_NAME, [settings]() -> std::shared_ptr<middleware::IMiddleware> {
			return std::make_shared<XFrameOptionsMiddleware>(settings);
		}},
		{CommonMiddleware::FULL_NAME, [settings]() -> std::shared_ptr<middleware::IMiddleware> {
			return std::make_shared<CommonMiddleware>(settings);
		}},
		{CookieMiddleware::FULL_NAME, [settings]() -> std::shared_ptr<middleware::IMiddleware> {
			return std::make_shared<CookieMiddleware>(settings);
		}},
		{ConditionalGetMiddleware::FULL_NAME, [settings]() -> std::shared_ptr<middleware::IMiddleware> {
			return std::make_shared<ConditionalGetMiddleware>(settings);
		}},
		{SecurityMiddleware::FULL_NAME, [settings]() -> std::shared_ptr<middleware::IMiddleware> {
			return std::make_shared<SecurityMiddleware>(settings);
		}}
	};

	using namespace render::lib;
	this->_libraries = {
		{BuiltinLibrary::FULL_NAME, [settings]() -> std::shared_ptr<ILibrary> {
			return std::make_shared<BuiltinLibrary>(settings);
		}}
	};
}

template <typename _T, typename>
void SettingsFactory::register_app(const std::string& full_name)
{
	if (this->_apps.find(full_name) != this->_apps.end())
	{
		if (this->_settings->LOGGER)
		{
			this->_settings->LOGGER->warning(
				"unable to register '" + full_name + "' app which already exists"
			);
		}
	}
	else
	{
		this->_apps[full_name] = [this]() -> std::shared_ptr<apps::IAppConfig> {
			return std::make_shared<_T>(this->_settings);
		};
	}
}

template <typename _T, typename>
void SettingsFactory::register_middleware(const std::string& full_name)
{
	if (this->_middleware.find(full_name) != this->_middleware.end())
	{
		if (this->_settings->LOGGER)
		{
			this->_settings->LOGGER->warning(
				"unable to register '" + full_name + "' middleware which already exists"
			);
		}
	}
	else
	{
		this->_middleware[full_name] = [this]() -> std::shared_ptr<middleware::IMiddleware> {
			return std::make_shared<_T>(this->_settings);
		};
	}
}

template <typename _T, typename>
void SettingsFactory::register_library(const std::string& full_name)
{
	if (this->_libraries.find(full_name) != this->_libraries.end())
	{
		if (this->_settings->LOGGER)
		{
			this->_settings->LOGGER->warning(
				"unable to register '" + full_name + "' library which already exists"
			);
		}
	}
	else
	{
		this->_libraries[full_name] = [this]() -> std::shared_ptr<render::lib::ILibrary> {
			return std::make_shared<_T>(this->_settings);
		};
	}
}

std::shared_ptr<apps::IAppConfig> SettingsFactory::get_app(
	const std::string& full_name
)
{
	if (this->_apps.find(full_name) != this->_apps.end())
	{
		return this->_apps[full_name]();
	}

	return nullptr;
}

std::shared_ptr<middleware::IMiddleware> SettingsFactory::get_middleware(
	const std::string& full_name
)
{
	if (this->_middleware.find(full_name) != this->_middleware.end())
	{
		return this->_middleware[full_name]();
	}

	return nullptr;
}

std::shared_ptr<render::lib::ILibrary> SettingsFactory::get_library(
	const std::string& full_name
)
{
	if (this->_libraries.find(full_name) != this->_libraries.end())
	{
		return this->_libraries[full_name]();
	}

	return nullptr;
}

__CONF_INTERNAL_END__
