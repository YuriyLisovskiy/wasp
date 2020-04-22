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
		this->_template_name = "form_app/form.html";
	}

	std::unique_ptr<wasp::http::IHttpResponse> get(wasp::http::HttpRequest* request, wasp::views::Args* args) final
	{
		using namespace wasp::render;
		using namespace wasp::core::types;

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

		auto ctx = std::make_shared<Context>(Context::scope_t{{
			"user_info",
			std::shared_ptr<Value<std::string>>(
				new Value<std::string>(user_row)
			)
		}});

		return this->render(request, ctx);
	}

	std::unique_ptr<wasp::http::IHttpResponse> post(wasp::http::HttpRequest* request, wasp::views::Args* args) final
	{
		if (request->FILES.contains("super_file"))
		{
			auto super_file = request->FILES.get("super_file");
			super_file.save();
		}

		return std::make_unique<wasp::http::HttpResponseRedirect>("/index");
	}
};
