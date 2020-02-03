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
 * An implementation of render/base.h.
 */

#include "./base.h"


__RENDER_BEGIN__

BaseTemplate* BaseEngine::from_string(const std::string &template_code)
{
	throw core::NotImplementedException(
		"subclasses of BaseEngine should provide a from_string() method",
		_ERROR_DETAILS_
	);
}

void BaseEngine::template_dirs(
	std::vector<std::string>& dirs, bool use_app_dirs
)
{
	// TODO: implement BaseEngine::template_dirs
}

__RENDER_END__
