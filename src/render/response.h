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
#include <xalwart.base/interfaces/render.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "../http/response.h"


__RENDER_BEGIN__

// TESTME: TemplateResponse
// TODO: docs for 'TemplateResponse'
class TemplateResponse : public http::Response
{
public:
	explicit TemplateResponse(
		render::IEngine* engine,
		const std::string& template_name,
		render::IContext* context=nullptr,
		unsigned short int status=200,
		const std::string& content_type="",
		const std::string& charset="utf-8"
	);

	void render();

	[[nodiscard]]
	std::string get_content() const final;

private:
	std::string _template_name;
	render::IContext* _context;
	render::IEngine* _engine;
	bool _is_rendered;
};

__RENDER_END__
