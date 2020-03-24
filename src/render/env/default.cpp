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
 * An implementation of render/backends/wasp.h
 */

#include "./default.h"


__ENV_BEGIN__

DefaultEnvironment::DefaultEnvironment(Config& config)
{
	if (!config.logger)
	{
		throw core::ImproperlyConfigured("DefaultEnvironment: logger must be instantiated.");
	}

	this->_use_app_dirs = config.use_app_dirs;
	this->_dirs = std::move(config.dirs);
	this->_dirs = this->template_dirs(config.apps);
	this->_engine = std::move(std::unique_ptr<render::IEngine>(new render::Engine(
		this,
		this->_dirs,
		this->_use_app_dirs,
		config.debug,
		config.auto_escape,
		config.loaders,
		config.libraries,
		config.logger
	)));

	this->_name = this->__type__().name();
}

std::string DefaultEnvironment::name()
{
	return this->_name;
}

std::shared_ptr<ITemplate> DefaultEnvironment::from_string(const std::string& template_code)
{
	return this->_engine->from_string(template_code);
}

std::shared_ptr<ITemplate> DefaultEnvironment::get_template(const std::string& template_path)
{
	return this->_engine->get_template(template_path);
}

std::string DefaultEnvironment::render(const std::string& src_code, IContext* ctx)
{
	return this->_engine->from_string(src_code)->render(ctx);
}

std::vector<std::string> DefaultEnvironment::template_dirs(
	const std::vector<std::shared_ptr<apps::IAppConfig>>& apps
)
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

void DefaultEnvironment::load_libs()
{
	this->_engine->load_libraries();
}

__ENV_END__
