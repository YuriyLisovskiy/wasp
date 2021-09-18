/**
 * render/library.h
 *
 * Copyright (c) 2020-2021 Yuriy Lisovskiy
 *
 * Purpose: TODO
 */

#pragma once

// Base libraries.
#include <xalwart.base/abc/render.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "../conf/settings.h"


__RENDER_BEGIN__

// TESTME: Library
// TODO: docs for 'Library'
class Library : public abc::render::ILibrary
{
public:
	inline explicit Library(conf::Settings* settings) : _settings(settings)
	{
	}

protected:
	[[nodiscard]]
	inline conf::Settings* settings() const
	{
		return require_non_null(this->_settings, "'settings' is nullptr", _ERROR_DETAILS_);
	}

private:
	conf::Settings* _settings;
};

__RENDER_END__
