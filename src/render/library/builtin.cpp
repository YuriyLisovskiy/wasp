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
 * An implementation of render/library/builtin.h
 */

// C++ libraries.
#include <map>
#include <memory>

#include "./builtin.h"

// Framework modules.
#include "../internal/syntax/url_tag.h"


__LIB_BEGIN__

BuiltinLibrary::BuiltinLibrary(conf::Settings* settings)
{
	if (!settings)
	{
		throw core::NullPointerException("BuiltinLibrary: 'settings' parameter is not initialized");
	}

	this->_settings = settings;
}

std::shared_ptr<Filters> BuiltinLibrary::get_filters()
{
	return nullptr;
}

std::shared_ptr<Tags> BuiltinLibrary::get_tags()
{
	return std::make_shared<Tags>(std::map<std::string, Tag>{
		{internal::syntax::TAG_NAME_URL, internal::syntax::make_url_tag(this->_settings->ROOT_URLCONF)}
	});
}

std::string BuiltinLibrary::name()
{
	return "builtin";
}

__LIB_END__
