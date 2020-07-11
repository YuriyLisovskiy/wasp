/*
 * Copyright (c) 2020 Yuriy Lisovskiy
 */

#pragma once

#include <xalwart/render/base.h>


class MyLoader : public xw::render::ILoader
{
private:
	std::map<std::string, std::string> _templates;

public:
	explicit MyLoader();
	std::shared_ptr<xw::render::ITemplate> get_template(
		const std::string& template_name,
		const std::vector<std::string>& dirs,
		xw::render::IEngine* engine
	) override;

	std::map<std::string, std::shared_ptr<xw::render::ITemplate>> cache_templates(
		const std::vector<std::string>& dirs,
		xw::render::IEngine* engine
	) override;
};
