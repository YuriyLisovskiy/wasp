/*
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 */

#pragma once

#include <memory>

#include "../../src/core/types/value.h"
#include "../../src/views/template_view.h"
#include "../../src/render/context.h"


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
			user_row += "User: " + args->get_str("user_name");
		}

		if (args->contains("user_id"))
		{
			user_row += ", ID: " + args->get_str("user_id");
		}

		if (user_row.empty())
		{
			user_row += "User is not found";
		}

		auto ctx = std::shared_ptr<wasp::render::Context>(new wasp::render::Context({{
			"user_info",
			std::shared_ptr<wasp::core::types::Value<std::string>>(
				new wasp::core::types::Value<std::string>(user_row)
			)
		}}));

		return this->render(request, ctx);
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
