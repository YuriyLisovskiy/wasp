/**
 * controllers/static.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * Controller for serving static files. These are only to be used
 * during development, and SHOULD NOT be used in a production setting.
 */

#pragma once

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "./controller.h"


__CONTROLLERS_BEGIN__

/// Checks if something was modified since the user last downloaded it.
extern bool was_modified_since(const std::string& header, size_t time, size_t size);

/// Serve static files below a given point in the directory structure.
class StaticController final : public Controller
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

	Result<std::shared_ptr<http::IHttpResponse>> get(Kwargs* kwargs) final;
};

__CONTROLLERS_END__
