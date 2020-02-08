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
 * An implementation of render/backends/base.h
 */

#include "./base.h"


__BACKENDS_BEGIN__

BaseBackend::BaseBackend(
	std::vector<std::string> dirs,
	bool use_app_dirs
) : _dirs(std::move(dirs)),
	_use_app_dirs(use_app_dirs)
{
	this->_name = this->get_type().name();
	core::str::rtrim(this->_name, "Backend");
	core::str::rtrim(this->_name, "backend");
	core::str::rtrim(this->_name, "_");
}

std::string BaseBackend::name()
{
	return this->_name;
}

ITemplate* BaseBackend::from_string(const std::string &template_code)
{
	throw core::NotImplementedException(
		"subclasses of BaseBackend should provide a from_string() method",
		_ERROR_DETAILS_
	);
}

std::vector<std::string> BaseBackend::template_dirs(
	const std::vector<apps::IAppConfig*>& apps
)
{
	auto dirs_copy = this->_dirs;
	if (this->_use_app_dirs)
	{
		for (const auto& app : apps)
		{
			dirs_copy.push_back(core::path::dirname(app->get_app_path()));
		}
	}

	return dirs_copy;
}

__BACKENDS_END__
