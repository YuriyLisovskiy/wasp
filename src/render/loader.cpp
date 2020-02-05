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
 * An implementation of render/loader.h.
 */

#include "./loader.h"


__RENDER_BEGIN__

ITemplate* Loader::get_template(
	const std::string& template_name,
	const std::vector<std::string>& dirs,
	BaseEngine* engine
)
{
	// TODO: implement Loader::get_template(
	//    const std::string& template_name,
	//    const std::vector<std::string>& dirs
	//  )
	return nullptr;
}

__RENDER_END__
