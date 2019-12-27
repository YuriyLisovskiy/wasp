/*
 * Copyright (c) 2019 Yuriy Lisovskiy
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
 * url.h
 * Purpose: utility functions for working with url patterns.
 */

#ifndef WASP_URLS_URL_H
#define WASP_URLS_URL_H

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "./pattern.h"
#include "../views/static.h"
#include "../core/exceptions.h"


__URLS_BEGIN__

extern UrlPattern make_url(
	const std::string& rgx, const views::ViewHandler& handler, const std::string& name
);

extern UrlPattern make_static(
	const std::string& static_url,
	const std::string& static_root,
	const std::string& name = ""
);

__URLS_END__


#endif // WASP_URLS_URL_H
