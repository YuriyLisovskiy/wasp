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
#include "../urls/pattern.h"


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

	Result<std::shared_ptr<http::IHttpResponse>> get(const std::string& p) final;
};

// TESTME: make_static
// TODO: docs for 'make_static'
inline std::shared_ptr<urls::IPattern> make_static(
	const std::string& static_url, const std::string& static_root, const std::string& name=""
)
{
	if (static_url.empty())
	{
		throw ImproperlyConfigured("empty static url not permitted", _ERROR_DETAILS_);
	}

	auto controller_func = [static_root](
		http::HttpRequest* request, const std::tuple<std::string>& args, conf::Settings* settings
	) -> Result<std::shared_ptr<http::IHttpResponse>>
	{
		ctrl::StaticController controller(settings);
		controller.set_kwargs(Kwargs({
			{"document_root", static_root}
		}));
		controller.setup(request);
		return std::apply(
			[controller](const std::string& p) mutable -> auto {return controller.dispatch(p); }, args
		);
	};

	return std::make_shared<urls::Pattern<std::string>>(
		str::rtrim(static_url, "/") + "/" + "<path>(.*)", controller_func, name.empty() ? "static" : name
	);
}

__CONTROLLERS_END__
