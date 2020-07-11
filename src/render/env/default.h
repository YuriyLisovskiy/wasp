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
 * render/env/default.h
 *
 * Purpose: default environment.
 */

#pragma once

// Module definitions.
#include "../_def_.h"

// Framework modules.
#include "./interfaces.h"


__ENV_BEGIN__

class DefaultEnvironment : public IEnvironment, public core::object::Object
{
protected:
	std::unique_ptr<IEngine> _engine;
	std::string _name;
	std::vector<std::string> _dirs;
	bool _use_app_dirs;

public:
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
		std::unique_ptr<IEngine> engine;

		explicit Config(
			std::unique_ptr<IEngine> engine,
			std::vector<std::string> dirs = {},
			bool use_app_dirs = true,
			std::vector<std::shared_ptr<apps::IAppConfig>> apps = {},
			bool debug = false,
			core::ILogger* logger = nullptr,
			bool auto_escape = true,
			std::vector<std::shared_ptr<render::lib::ILibrary>> libraries = {},
			std::vector<std::shared_ptr<ILoader>> loaders = {}
		)
		{
			this->debug = debug;
			this->logger = logger;
			this->loaders = std::move(loaders);
			this->libraries = std::move(libraries);
			this->auto_escape = auto_escape;
			this->dirs = std::move(dirs);
			this->use_app_dirs = use_app_dirs;
			this->apps = std::move(apps);
			this->engine = std::move(engine);
		}

		std::unique_ptr<IEnvironment> make_env()
		{
			return std::make_unique<env::DefaultEnvironment>(*this);
		}
	};

	explicit DefaultEnvironment(Config& config);

	std::string name() override;

	std::shared_ptr<ITemplate> from_string(const std::string& template_code) override;
	std::shared_ptr<ITemplate> get_template(const std::string& template_path) override;
	std::string render(const std::string& src_code, IContext* ctx) override;

	/// Initializes a std::vector of directories to search for templates.
	std::vector<std::string> template_dirs(
		const std::vector<std::shared_ptr<apps::IAppConfig>>& apps
	) override;

	void load_libs() override;
};

__ENV_END__
