/**
 * render/default.cpp
 *
 * Copyright (c) 2020-2021 Yuriy Lisovskiy
 */

#include "./default_library.h"

// Framework libraries.
#include "./tags/static_tag.h"
#include "./tags/url_tag.h"


__RENDER_BEGIN__

std::map<std::string, Tag> DefaultLibrary::get_tags() const
{
	return {
		// Example: {% static('path/to/file.css') -> css_variable %}
		{tags::TAG_NAME_STATIC, {tags::make_static_tag("static", this->settings()->STATIC_URL)}},

		// Example: {% media('path/to/file.jpg') -> jpg_variable %}
		{tags::TAG_NAME_MEDIA, {tags::make_static_tag("media", this->settings()->MEDIA_URL)}},

		// Example: {% url('app_namespace::profile', 256) -> profile_256 %}
		{tags::TAG_NAME_URL, {tags::make_url_tag(this->settings()->URLPATTERNS)}}
	};
}

__RENDER_END__
