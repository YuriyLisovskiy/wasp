/**
 * controllers/static.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * Controller for serving static files. These are only to be used
 * during development, and SHOULD NOT be used in a production setting.
 */

#pragma once

// Base libraries.
#include <xalwart.base/kwargs.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "./controller.h"


__CONTROLLERS_BEGIN__

// TESTME: was_modified_since
// Checks if something was modified since the user last downloaded it.
extern bool was_modified_since(const std::string& header, size_t time, size_t size);

// TESTME: StaticController
// TODO: docs for 'StaticController'
// Serve static files below a given point in the directory structure.
class StaticController final : public Controller<const std::string&>
{
private:
	Kwargs _kwargs;

public:
	inline explicit StaticController(conf::Settings* settings) : Controller({"get"}, settings)
	{
	}

	inline void set_kwargs(const Kwargs& kwargs)
	{
		this->_kwargs = kwargs;
	}

	std::unique_ptr<http::abc::IHttpResponse> get(const std::string& p) final;
};

__CONTROLLERS_END__
