/**
 * urls/url.h
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 *
 * Purpose:  utility functions for working with url patterns.
 */

#pragma once

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "./pattern.h"
#include "../views/static.h"


__URLS_BEGIN__

extern std::shared_ptr<urls::UrlPattern> make_url(
	const std::string& rgx,
	const views::ViewHandler& handler,
	const std::string& name
);

extern std::shared_ptr<urls::UrlPattern> make_static(
	const std::string& static_url,
	const std::string& static_root,
	const std::string& name = ""
);

__URLS_END__
