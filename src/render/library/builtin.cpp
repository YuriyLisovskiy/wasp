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

#include "./builtin.h"

// Framework modules.
#include "../tags/static_tag.h"
#include "../tags/url_tag.h"


__LIB_BEGIN__

const std::string DefaultLibrary::FULL_NAME = "xw::render::lib::DefaultLibrary";

DefaultLibrary::DefaultLibrary(conf::Settings* settings)
	: Library(DefaultLibrary::FULL_NAME, settings)
{
}

std::shared_ptr<Filters> DefaultLibrary::get_filters()
{
	return nullptr;
}

std::shared_ptr<Tags> DefaultLibrary::get_tags()
{
	using namespace internal;
	return std::make_shared<Tags>(std::map<std::string, Tag>{

		// Example: {% static('path/to/file.css') -> css_variable %}
		{tags::TAG_NAME_STATIC, tags::make_static_tag("static", this->settings->STATIC_URL)},

		// Example: {% media('path/to/file.jpg') -> jpg_variable %}
		{tags::TAG_NAME_MEDIA, tags::make_static_tag("media", this->settings->MEDIA_URL)},

		// Example: {% url('app_namespace::profile', 256) -> profile_256 %}
		{tags::TAG_NAME_URL, tags::make_url_tag(this->settings->ROOT_URLCONF)}
	});
}

std::string DefaultLibrary::name()
{
	return DefaultLibrary::FULL_NAME;
}

__LIB_END__
