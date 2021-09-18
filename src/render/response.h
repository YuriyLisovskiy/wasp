/**
 * render/response.h
 *
 * Copyright (c) 2020-2021 Yuriy Lisovskiy
 *
 * Http response which can render it's content using
 * configured backend.
 */

#pragma once

// Base libraries.
#include <xalwart.base/abc/render.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "../http/response.h"


__RENDER_BEGIN__

// TESTME: TemplateResponse
// TODO: docs for 'TemplateResponse'
class TemplateResponse : public http::Response
{
protected:
	std::string template_name;
	abc::render::IContext* context;
	abc::render::IEngine* engine;
	bool is_rendered;

public:
	explicit TemplateResponse(
		abc::render::IEngine* engine,
		const std::string& template_name,
		abc::render::IContext* context=nullptr,
		unsigned short int status=200,
		const std::string& content_type="",
		const std::string& charset="utf-8"
	);

	void render();

	[[nodiscard]]
	std::string get_content() const override;
};

__RENDER_END__
