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
 * static.h
 * Purpose: views and functions for serving static files. These are only to be used
			during development, and SHOULD NOT be used in a production setting.
 */

#ifndef WASP_VIEWS_STATIC_H
#define WASP_VIEWS_STATIC_H

// C++ libraries.
#include <regex>

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "./args.h"
#include "./view.h"
#include "../http/headers.h"
#include "../http/request.h"
#include "../http/response.h"
#include "../http/utility.h"
#include "../utility/logger.h"
#include "../utility/mime_types.h"
#include "../core/files/file.h"
#include "../core/datetime/datetime.h"
#include "../core/exceptions.h"


__VIEWS_BEGIN__

/// Serve static files below a given point in the directory structure.
class StaticView final : public View
{
private:
	collections::Dict<std::string, std::string>* _kwargs = nullptr;

public:
	explicit StaticView(wasp::utility::ILogger* logger = nullptr) : View({"get"}, logger) {}

	void set_kwargs(collections::Dict<std::string, std::string>* kwargs);

	http::HttpResponseBase* get(http::HttpRequest* request, wasp::views::Args* args) final;
};

__VIEWS_END__


__VIEWS_INTERNAL_BEGIN__

/// Checks if something was modified since the user last downloaded it.
extern bool was_modified_since(const std::string& header, size_t time, size_t size);

__VIEWS_INTERNAL_END__


#endif // WASP_VIEWS_STATIC_H
