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


template<class... Args>
void print(Args&&... args)
{
	(std::cout << ... << args) << "\n";
}


class FormView : public wasp::View
{
public:
	explicit FormView(wasp::ILogger* logger = nullptr) : View({"get", "post"}, logger) {}

	wasp::HttpResponse* get(wasp::HttpRequest* request, wasp::Args* args) final
	{
		auto request_body = request->body();
		std::cout << request_body << '\n';

		print(
			"\nGet: ", request->GET.keys().size(),
			"\nPost: ", request->POST.keys().size(),
			"\nFiles: ", request->FILES.keys().size(),
			"\nCookies: ", request->COOKIES.keys().size()
		);
		print("\nCOOKIES: ", request->headers.get("Cookie"));

		std::string body(
			"<form action=\"/hello\" method=\"post\" enctype=\"multipart/form-data\">\n"
			"\t<input type=\"file\" name=\"super_file\" />\n"
			"\t<input type=\"email\" name=\"mail\" />\n"
			"\t<input type=\"text\" name=\"name\" />\n"
			"\t<input type=\"number\" name=\"birth_year\" />\n"
			"\t<input type=\"submit\" value=\"send\" />\n"
			"\t</form>\n"
		);

		return new wasp::HttpResponse(body);
	}

	wasp::HttpResponse* post(wasp::HttpRequest* request, wasp::Args* args) final
	{
		return new wasp::HttpResponseRedirect("/hello");
	}
};


#endif // WASP_FORM_VIEW_H
