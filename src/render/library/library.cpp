/**
 * render/library/library.cpp
 *
 * Copyright (c) 2020 Yuriy Lisovskiy
 */

#include "./library.h"


__LIB_BEGIN__

Library::Library(const std::string& lib_name, conf::Settings* settings)
{
	if (!settings)
	{
		throw core::ImproperlyConfigured(
			lib_name + ": 'settings' parameter must be initialized"
		);
	}

	this->settings = settings;
}

std::shared_ptr<Filters> Library::get_filters()
{
	return nullptr;
}

std::shared_ptr<Tags> Library::get_tags()
{
	return nullptr;
}

__LIB_END__
