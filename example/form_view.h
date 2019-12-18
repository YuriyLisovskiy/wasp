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

#ifndef WASP_FORM_VIEW_H
#define WASP_FORM_VIEW_H

#include "../src/views/generic.h"
#include "../src/utility/logger.h"

using wasp::http::HttpRequest;
using wasp::http::HttpResponse;
using wasp::http::HttpResponseBase;


template<class... Args>
void print(Args&&... args)
{
	(std::cout << ... << args) << "\n";
}


class FormView : public wasp::views::View
{
public:
	explicit FormView(wasp::utility::ILogger* logger = nullptr) : View({"get", "post"}, logger) {}

	HttpResponseBase* get(HttpRequest* request, wasp::views::Args* args) final
	{
		std::cout << "\n\n" << request->path();

		if (request->FILES.contains("super_file"))
		{
			auto super_file = request->FILES.get("super_file");
			super_file.save();
		}

		std::string user_row;
		if (args->contains("user_name"))
		{
			user_row += "<h3>User: " + args->get_str("user_name");
		}

		if (args->contains("user_id"))
		{
			user_row += ", ID: " + args->get_str("user_id");
		}

		if (!user_row.empty())
		{
			user_row += "</h3>\n";
		}

		std::string body(
			user_row +
			"<img src=\"/static/pontar.png\" alt=\"Night over Pontar river\" height=\"100\" width=\"100\">"
			"<form action=\"/hello\" method=\"post\" enctype=\"multipart/form-data\">\n"
			"\t<input type=\"file\" name=\"super_file\" />\n"
			"\t<input type=\"email\" name=\"mail\" />\n"
			"\t<input type=\"text\" name=\"name\" />\n"
			"\t<input type=\"number\" name=\"birth_year\" />\n"
			"\t<input type=\"submit\" value=\"send\" />\n"
			"\t</form>\n"
		);

		return new HttpResponse(body);
	}

	HttpResponseBase* post(HttpRequest* request, wasp::views::Args* args) final
	{
		return this->get(request, args);
	}
};


#endif // WASP_FORM_VIEW_H
