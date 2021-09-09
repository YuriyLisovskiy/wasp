/**
 * middleware/base.h
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 *
 * Base middleware with settings field.
 */

#pragma once

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "../conf/settings.h"


__MIDDLEWARE_BEGIN__

// TESTME: MiddlewareWithConstantSettings
// TODO: docs for 'MiddlewareWithConstantSettings'
class MiddlewareWithConstantSettings
{
public:
	inline explicit MiddlewareWithConstantSettings(const conf::Settings* settings) : settings(settings)
	{
		require_non_null(settings, "'settings' is nullptr", _ERROR_DETAILS_);
	}

	virtual ~MiddlewareWithConstantSettings() = default;

protected:
	const conf::Settings* settings;
};

__MIDDLEWARE_END__
