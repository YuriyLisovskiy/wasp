/*
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 */

#pragma once

#include "../../src/views/view.h"
#include "../../src/core/logger.h"
#include "../../src/apps/config.h"


class FormView : public wasp::views::View
{
public:
	explicit FormView(wasp::conf::Settings* settings)
		: wasp::views::View({"get", "post"}, settings)
	{
	}

	wasp::http::HttpResponseBase* get(wasp::http::HttpRequest* request, wasp::views::Args* args) final
	{
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
			"<form method=\"post\" enctype=\"multipart/form-data\">\n"
			"\t<input type=\"file\" name=\"super_file\" />\n"
			"\t<input type=\"email\" name=\"mail\" />\n"
			"\t<input type=\"text\" name=\"name\" />\n"
			"\t<input type=\"number\" name=\"birth_year\" />\n"
			"\t<input type=\"submit\" value=\"send\" />\n"
			"\t</form>\n"
		);

		return new wasp::http::HttpResponse(body);
	}

	wasp::http::HttpResponseBase* post(wasp::http::HttpRequest* request, wasp::views::Args* args) final
	{
		if (request->FILES.contains("super_file"))
		{
			auto super_file = request->FILES.get("super_file");
			super_file.save();
		}

		return new wasp::http::HttpResponseRedirect("/index");
	}
};
