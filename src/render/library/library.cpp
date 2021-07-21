/**
 * render/library/library.cpp
 *
 * Copyright (c) 2020-2021 Yuriy Lisovskiy
 */

#include "./library.h"


__RENDER_BEGIN__

Library::Library(const std::string& lib_name, conf::Settings* settings)
{
	if (!settings)
	{
		throw ImproperlyConfigured(
			lib_name + ": 'settings' parameter must be initialized",
			_ERROR_DETAILS_
		);
	}

	this->settings = settings;
}

std::shared_ptr<collections::Dict<std::string, filter_t>> Library::get_filters()
{
	return nullptr;
}

std::shared_ptr<collections::Dict<std::string, tag_t>> Library::get_tags()
{
	return nullptr;
}

__RENDER_END__
