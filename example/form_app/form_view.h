/*
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 */

#pragma once

#include "../../src/core/logger.h"
#include "../../src/apps/config.h"
#include "../../src/views/template_view.h"


class FormView : public wasp::views::TemplateView
{
public:
	explicit FormView(wasp::conf::Settings* settings)
		: wasp::views::TemplateView({"get", "post"}, settings)
	{
		this->_template_name = "form.html";
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

		return this->render(request);
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
