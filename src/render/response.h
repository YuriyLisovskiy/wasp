/**
 * render/response.h
 *
 * Copyright (c) 2020-2021 Yuriy Lisovskiy
 *
 * Purpose:
 * 	Http response which can render it's content using
 * 	configured backend.
 */

#pragma once

// Module definitions.
#include <xalwart.render/_def_.h>

// Render libraries.
#include <xalwart.render/abc.h>

// Framework libraries.
#include "../http/response.h"


__RENDER_BEGIN__

class TemplateResponse : public http::HttpResponse
{
protected:
	std::string template_name;
	IContext* context;
	render::IEngine* engine;
	bool is_rendered;

public:
	explicit TemplateResponse(
		render::IEngine* engine,
		const std::string& template_name,
		IContext* context = nullptr,
		unsigned short int status = 200,
		const std::string& content_type = "",
		const std::string& charset = "utf-8"
	);

	void render();
	std::string get_content() override;
};

__RENDER_END__
