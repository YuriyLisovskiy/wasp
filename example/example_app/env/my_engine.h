/*
 * Copyright (c) 2020 Yuriy Lisovskiy
 */

#pragma once

#include <xalwart/render/base_engine.h>
#include <xalwart/render/env/interfaces.h>


class MyEngine : public xw::render::BaseEngine
{
private:
	xw::render::env::IEnvironment* _env;
	std::vector<std::shared_ptr<xw::render::ILoader>> _loaders;

	// Just for overridden methods.
	xw::render::lib::Filters _filters;
	xw::render::lib::Tags _tags;

public:
	explicit MyEngine(
		xw::render::env::IEnvironment* env,
		xw::render::env::Config* cfg
	);
	std::shared_ptr<xw::render::ITemplate> find_template(
		const std::string& name,
		const std::vector<std::string>& dirs
	);
	std::shared_ptr<xw::render::ITemplate> from_string(
		const std::string& template_code
	) override;
	std::shared_ptr<xw::render::ITemplate> get_template(
		const std::string& template_name
	) override;
	xw::render::env::IEnvironment* environment();
	xw::render::lib::Filters& get_filters() override;
	xw::render::lib::Tags& get_tags() override;
	void load_libraries() override;
};
