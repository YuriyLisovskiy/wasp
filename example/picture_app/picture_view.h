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
#pragma once

#include <string>

#include "../../src/views/view.h"


class PictureView : public wasp::views::View
{
public:
	explicit PictureView(wasp::conf::Settings* settings)
		: View({"get"}, settings)
	{
	}

	wasp::http::HttpResponseBase* get(wasp::http::HttpRequest* request, wasp::views::Args* args) final
	{
		std::string body(R"(<img src="/static/pontar.png" alt="Night over Pontar river" width="1000">)");
		return new wasp::http::HttpResponse(body);
	}
};
