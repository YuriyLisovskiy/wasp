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

#include "./wasp.h"


__BACKENDS_BEGIN__

WaspBackend::WaspBackend(
	const std::vector<std::string>& dirs,
	bool use_app_dirs,
	const std::vector<apps::IAppConfig*>& installed_apps,
	const Options& opts
) : BaseBackend(dirs, use_app_dirs)
{
	if (!opts.logger)
	{
		throw core::ImproperlyConfigured("WaspBackend: logger must be instantiated.");
	}

	this->_dirs = this->template_dirs(installed_apps);
	this->_engine = new render::Engine(
		this,
		this->_dirs,
		this->_use_app_dirs,
		opts.debug,
		opts.auto_escape,
		opts.loaders,
		opts.filters,
		opts.logger
	);

	this->_name = this->get_type().name();
	core::str::rtrim(this->_name, "Backend");
	core::str::rtrim(this->_name, "backend");
	core::str::rtrim(this->_name, "_");
}

WaspBackend::~WaspBackend()
{
	delete this->_engine;
}

ITemplate* WaspBackend::from_string(const std::string& template_code)
{
	return this->_engine->from_string(template_code);
}

ITemplate* WaspBackend::get_template(const std::string& template_path)
{
	return this->_engine->get_template(template_path);
}

__BACKENDS_END__
