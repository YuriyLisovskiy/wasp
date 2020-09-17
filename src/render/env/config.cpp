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
 * An implementation of render/env/config.h
 */

// Header.
#include "./config.h"


__ENV_BEGIN__

Config::Config(
	std::vector<std::string> dirs,
	bool use_app_dirs,
	const std::vector<std::shared_ptr<apps::IAppConfig>>& apps,
	bool debug,
	core::ILogger* logger,
	bool auto_escape,
	std::vector<std::shared_ptr<render::lib::ILibrary>> libraries,
	std::vector<std::shared_ptr<ILoader>> loaders
)
{
	this->debug = debug;
	this->logger = logger;
	this->loaders = std::move(loaders);
	this->libraries = std::move(libraries);
	this->auto_escape = auto_escape;
	this->dirs = std::move(dirs);
	this->use_app_dirs = use_app_dirs;
	this->apps = apps;
}

__ENV_END__
