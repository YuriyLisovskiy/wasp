/*
 * Copyright (c) 2020 Yuriy Lisovskiy
 */

#include "./my_engine.h"

#include <xalwart/render/template.h>

#include "./loaders/my_loader.h"


MyEngine::MyEngine(
	xw::render::env::IEnvironment* env,
    xw::render::env::Config*
)
{
	this->_env = env;
	this->_loaders = std::vector<std::shared_ptr<xw::render::ILoader>>{
		std::make_shared<MyLoader>()
	};
}

std::shared_ptr<xw::render::ITemplate> MyEngine::find_template(
	const std::string& name,
	const std::vector<std::string>& dirs
)
{
	return this->_loaders[0]->get_template(name, dirs, this);
}

std::shared_ptr<xw::render::ITemplate> MyEngine::from_string(const std::string& template_code)
{
	return std::make_shared<xw::render::Template>(template_code, this);
}

std::shared_ptr<xw::render::ITemplate> MyEngine::get_template(const std::string& template_name)
{
	return this->find_template(template_name, {});
}

xw::render::env::IEnvironment* MyEngine::environment()
{
	return this->_env;
}

xw::render::lib::Filters& MyEngine::get_filters()
{
	return this->_filters;
}

xw::render::lib::Tags& MyEngine::get_tags()
{
	return this->_tags;
}

void MyEngine::load_libraries()
{
}
