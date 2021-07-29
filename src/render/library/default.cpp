/**
 * render/library/default.cpp
 *
 * Copyright (c) 2020-2021 Yuriy Lisovskiy
 */

#include "./default.h"

// Framework libraries.
#include "../tags/static_tag.h"
#include "../tags/url_tag.h"


__RENDER_BEGIN__

const std::string DefaultLibrary::FULL_NAME = "xw::render::DefaultLibrary";

DefaultLibrary::DefaultLibrary(conf::Settings* settings)
	: Library(DefaultLibrary::FULL_NAME, settings)
{
}

std::shared_ptr<collections::Dictionary<std::string, filter_t>> DefaultLibrary::get_filters()
{
	return nullptr;
}

std::shared_ptr<collections::Dictionary<std::string, tag_t>> DefaultLibrary::get_tags()
{
	using namespace internal;
	return std::make_shared<collections::Dictionary<std::string, tag_t>>(std::map<std::string, tag_t>{

		// Example: {% static('path/to/file.css') -> css_variable %}
		{tags::TAG_NAME_STATIC, {tags::make_static_tag("static", this->settings->STATIC_URL)}},

		// Example: {% media('path/to/file.jpg') -> jpg_variable %}
		{tags::TAG_NAME_MEDIA, {tags::make_static_tag("media", this->settings->MEDIA_URL)}},

		// Example: {% url('app_namespace::profile', 256) -> profile_256 %}
		{tags::TAG_NAME_URL, {tags::make_url_tag(this->settings->ROOT_URLCONF)}}
	});
}

std::string DefaultLibrary::name()
{
	return DefaultLibrary::FULL_NAME;
}

__RENDER_END__
