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
private:
	conf::Settings* _settings;

protected:
	inline conf::Settings* settings() const
	{
		if (!this->_settings)
		{
			throw NullPointerException("'settings' is nullptr", _ERROR_DETAILS_);
		}

		return this->_settings;
	}

public:
	inline explicit Library(conf::Settings* settings) : _settings(settings)
	{
	}

	inline std::map<std::string, Filter> get_filters() const override
	{
		return {};
	}

	inline std::map<std::string, Tag> get_tags() const override
	{
		return {};
	}
};

__RENDER_END__
