/*
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * urls/url.h
 *
 * Purpose: utility functions for working with url patterns.
 */

#pragma once

// C++ libraries.
#include <memory>

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "./pattern.h"
#include "../views/static.h"
#include "../core/exceptions.h"
#include "../conf/settings.h"


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
