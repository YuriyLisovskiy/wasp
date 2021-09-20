/**
 * render/standard_library.cpp
 *
 * Copyright (c) 2020-2021 Yuriy Lisovskiy
 */

#include "./standard_library.h"

// Framework libraries.
#include "./functions.h"


__RENDER_BEGIN__

std::map<std::string, abc::render::ILibrary::Function> StandardLibrary::get_functions() const
{
	return {
		// Example: {% static('path/to/file.css') -> css_variable %}
		{"static", make_static_function(this->settings()->STATIC.URL)},

		// Example: {% media('path/to/file.jpg') -> jpg_variable %}
		{"media", make_static_function(this->settings()->STATIC.URL)},

		// Example: {% url('app_namespace::profile', 256) -> profile_256 %}
		{"url", make_url_function(this->settings()->URLPATTERNS)}
	};
}

__RENDER_END__
