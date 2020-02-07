/*
 * Copyright (c) 2020 Yuriy Lisovskiy
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

/**
 * An implementation of views/template_view.h
 */

#include "./template_view.h"


__VIEWS_BEGIN__

TemplateResponseMixin::TemplateResponseMixin(
	render::backends::BaseBackend* backend,
	bool auto_delete_context
)
{
	if (!backend)
	{
		throw core::ImproperlyConfigured(
			"Backend must be initialized in order to use the application",
			_ERROR_DETAILS_
		);
	}

	this->_backend = backend;
	this->_template_name = "";
	this->_content_type = "";
	this->_auto_delete_context = auto_delete_context;
}

render::TemplateResponse* TemplateResponseMixin::render(
	http::HttpRequest* request,
	const std::string& template_name,
	render::IContext* context,
	unsigned short int status,
	const std::string& content_type,
	const std::string& charset
)
{
	auto* response = new render::TemplateResponse(
		this->_backend,
		template_name.empty() ? this->get_template_name() : template_name,
		context,
		status,
		content_type,
		charset
	);
	response->render();
	if (this->_auto_delete_context)
	{
		delete context;
	}

	return response;
}

std::string TemplateResponseMixin::get_template_name()
{
	if (this->_template_name.empty())
	{
		throw core::ImproperlyConfigured(
			"TemplateResponseMixin requires either a definition of '_template_name' or an "
			"implementation of 'get_template_name()'"
		);
	}

	return this->_template_name;
}


TemplateView::TemplateView(
	conf::Settings* settings
) : views::View({"get", "options"}, settings),
	TemplateResponseMixin(nullptr, true)
{
}

TemplateView::TemplateView(
	const std::vector<std::string>& allowed_methods,
	conf::Settings* settings,
	bool auto_delete_context
) : views::View(allowed_methods, settings),
    TemplateResponseMixin(nullptr, auto_delete_context)
{
}

render::IContext* TemplateView::get_context()
{
	return nullptr;
}

http::HttpResponseBase* TemplateView::get(
	http::HttpRequest* request, Args* args
)
{
	return this->render(request, "", this->get_context());
}

__VIEWS_END__
