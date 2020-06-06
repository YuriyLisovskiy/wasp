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
#include "./settings.h"
#include "../render/library/base.h"
#include "../middleware/interfaces.h"


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

public:
	SettingsFactory(nullptr_t) = delete;
	SettingsFactory(Settings* settings);

	template <typename _T, typename = std::enable_if<std::is_base_of<apps::IAppConfig, _T>::value>>
	void register_app(const std::string& full_name);

	template <typename _T, typename = std::enable_if<std::is_base_of<middleware::IMiddleware, _T>::value>>
	void register_middleware(const std::string& full_name);

	template <typename _T, typename = std::enable_if<std::is_base_of<render::lib::ILibrary, _T>::value>>
	void register_library(const std::string& full_name);

	std::shared_ptr<apps::IAppConfig> get_app(const std::string& full_name);
	std::shared_ptr<middleware::IMiddleware> get_middleware(const std::string& full_name);
	std::shared_ptr<render::lib::ILibrary> get_library(const std::string& full_name);
};

__CONF_INTERNAL_END__
