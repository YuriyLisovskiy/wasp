/**
 * render/library.h
 *
 * Copyright (c) 2020-2021 Yuriy Lisovskiy
 *
 * Purpose: TODO
 */

#pragma once

// Render libraries.
#include <xalwart.render/library/abc.h>

// Framework libraries.
#include "../conf/settings.h"


__RENDER_BEGIN__

// TESTME: Library
// TODO: docs for 'Library'
class Library : public abc::ILibrary
{
protected:
	conf::Settings* settings;

public:
	inline explicit Library(conf::Settings* settings) : settings(settings)
	{
		if (!this->settings)
		{
			throw NullPointerException("'settings' is nullptr", _ERROR_DETAILS_);
		}
	}

	inline std::shared_ptr<collections::Dictionary<std::string, filter_t>> get_filters() override
	{
		return nullptr;
	}

	inline std::shared_ptr<collections::Dictionary<std::string, tag_t>> get_tags() override
	{
		return nullptr;
	}
};

__RENDER_END__
