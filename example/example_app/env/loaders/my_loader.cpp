/*
 * Copyright (c) 2020 Yuriy Lisovskiy
 */

#include "./my_loader.h"

#include <xalwart/render/exceptions.h>
#include <xalwart/render/template.h>


MyLoader::MyLoader()
{
	this->_templates = std::map<std::string, std::string>{
		{"index.html", "<p>This is index page!</p>"},
		{"greetings.html", "<h3>Hello, {{ user_name }}<h3>"},
	};
}

std::shared_ptr<xw::render::ITemplate> MyLoader::get_template(
	const std::string& template_name,
	const std::vector<std::string>& dirs,
	xw::render::IEngine* engine
)
{
	if (this->_templates.find(template_name) != this->_templates.end())
	{
		return std::make_shared<xw::render::Template>(
			this->_templates[template_name], (xw::render::BaseEngine*)engine
		);
	}

	throw xw::render::TemplateDoesNotExist(
		template_name, dirs, nullptr, _ERROR_DETAILS_
	);
}

std::map<std::string, std::shared_ptr<xw::render::ITemplate>> MyLoader::cache_templates(
	const std::vector<std::string>& dirs,
	xw::render::IEngine* engine
)
{
	return {};
}
