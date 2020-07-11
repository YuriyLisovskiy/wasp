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
#include "./config.h"
#include "../../core/path.h"


__ENV_BEGIN__

template <typename EngineT>
class DefaultEnvironment : public IEnvironment
{
protected:
	std::unique_ptr<IEngine> _engine;
	std::vector<std::string> _dirs;
	bool _use_app_dirs;

public:
	explicit DefaultEnvironment(env::Config* cfg)
	{
		if (!cfg->logger)
		{
			throw core::ImproperlyConfigured("DefaultEnvironment: logger must be instantiated.");
		}

		this->_use_app_dirs = cfg->use_app_dirs;
		this->_dirs = cfg->dirs;
		this->_dirs = this->template_dirs(cfg->apps);
		cfg->dirs = this->_dirs;
		this->_engine = std::move(std::make_unique<EngineT>(this, cfg));
	}

	static std::unique_ptr<IEnvironment> make(env::Config* cfg)
	{
		return std::make_unique<env::DefaultEnvironment<EngineT>>(cfg);
	}

	std::string name() override
	{
		return "XalwartEngine";
	}

	std::shared_ptr<ITemplate> from_string(const std::string& template_code) override
	{
		return this->_engine->from_string(template_code);
	}

	std::shared_ptr<ITemplate> get_template(const std::string& template_path) override
	{
		return this->_engine->get_template(template_path);
	}

	std::string render(const std::string& src_code, IContext* ctx) override
	{
		return this->_engine->from_string(src_code)->render(ctx);
	}

	/// Initializes a std::vector of directories to search for templates.
	std::vector<std::string> template_dirs(
		const std::vector<std::shared_ptr<apps::IAppConfig>>& apps
	) override
	{
		if (this->_use_app_dirs)
		{
			auto dirs_copy = this->_dirs;
			for (const auto &app : apps)
			{
				dirs_copy.push_back(core::path::dirname(app->get_app_path()));
			}

			return dirs_copy;
		}
		else
		{
			return this->_dirs;
		}
	}

	void load_libs() override
	{
		this->_engine->load_libraries();
	}
};

__ENV_END__
