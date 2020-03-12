/*
 * Copyright (c) 2020 Yuriy Lisovskiy
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
 * render/response.h
 *
 * Purpose:
 * 	Http response which can render it's content using
 * 	configured backend.
 */

#pragma once

// C++ libraries.
#include <string>
#include <memory>

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "./base.h"
#include "./backends/base.h"
#include "./exceptions.h"
#include "../http/response.h"


__RENDER_BEGIN__

class TemplateResponse : public http::HttpResponse
{
protected:
	std::string _template_name;
	std::shared_ptr<IContext> _context;
	backends::IBackend* _backend;
	bool _is_rendered;

public:
	explicit TemplateResponse(
		backends::IBackend* backend,
		const std::string& template_name,
		const std::shared_ptr<IContext>& context = nullptr,
		unsigned short int status = 200,
		const std::string& content_type = "",
		const std::string& charset = "utf-8"
	);

	void render();
	std::string get_content() override;
};

__RENDER_END__
