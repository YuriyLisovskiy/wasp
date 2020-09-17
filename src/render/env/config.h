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
 * render/env/config.h
 *
 * Purpose:
 *  TODO: implement docs for render/env/config.h
 */

#pragma once

// Module definitions.
#include "./_def_.h"

// Framework modules.
#include <xalwart.render/library/base.h>
#include "../../apps/interfaces.h"


__ENV_BEGIN__

struct Config final
{
	bool debug;
	core::ILogger* logger;
	std::vector<std::shared_ptr<ILoader>> loaders;
	std::vector<std::shared_ptr<render::lib::ILibrary>> libraries;
	bool auto_escape;
	std::vector<std::string> dirs;
	bool use_app_dirs;
	std::vector<std::shared_ptr<apps::IAppConfig>> apps;

	explicit Config(
		std::vector<std::string> dirs = {},
		bool use_app_dirs = true,
		const std::vector<std::shared_ptr<apps::IAppConfig>>& apps = {},
		bool debug = false,
		core::ILogger* logger = nullptr,
		bool auto_escape = true,
		std::vector<std::shared_ptr<render::lib::ILibrary>> libraries = {},
		std::vector<std::shared_ptr<ILoader>> loaders = {}
	);
};

__ENV_END__
