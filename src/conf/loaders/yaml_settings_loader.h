/**
 * conf/loaders/yaml_settings_loader.h
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 *
 * Purpose: TODO
 */

#pragma once

// Core libraries.
#include <xalwart.core/yaml/yaml-cpp/yaml.h>

// Module definitions.
#include "../_def_.h"

// Framework libraries.
#include "./base_settings_loader.h"


__CONF_BEGIN__

class YamlSettingsLoader : public BaseSettingsLoader<YAML::Node>
{
private:
	static void _init_allowed_hosts(Settings* settings, const YAML::Node& allowed_hosts);

	static void _init_disallowed_user_agents(Settings* settings, const YAML::Node& agents);

	static void _init_ignorable_404_urls(Settings* settings, const YAML::Node& urls);

	static void _init_formats(Settings* settings, const YAML::Node& config);

	static void _init_csrf(Settings* settings, const YAML::Node& config);

	static void _init_secure(Settings* settings, const YAML::Node& config);

	static void _init_apps(Settings* settings, const YAML::Node& apps);

	static void _init_middleware(Settings* settings, const YAML::Node& middleware);

protected:

	// Returns YAML::Node of type Null
	[[nodiscard]]
	YAML::Node null() const;

	// Returns YAML::Node of type Map
	[[nodiscard]]
	YAML::Node map_node() const;

	virtual void check_config(const YAML::Node& config, const std::string& file_path);

	void overwrite_scalar_or_remove_if_null(
		YAML::Node& config, const YAML::Node& local_config, const std::string& key
	);
	void overwrite_sequence_or_remove_if_null(
		YAML::Node& config, const YAML::Node& local_config, const std::string& key
	);

	// Override this method to setup custom logger with custom parameters.
	virtual void overwrite_logger(YAML::Node& logger_cfg, const YAML::Node& local_logger_cfg);

	// Override this method to setup custom render engine with custom parameters.
	virtual void overwrite_template_engine(
		YAML::Node& engine_cfg, const YAML::Node& local_engine_cfg
	);

	virtual void init_logger_setting(Settings* settings, const YAML::Node& logger_config);

	virtual void init_template_engine_setting(Settings* settings, const YAML::Node& engine_config);

public:

	// Override this method to change the default config name.
	[[nodiscard]]
	std::string config_name() const override;

	// Override this method to change the default local config name.
	[[nodiscard]]
	std::string local_config_name() const override;

	// Reads and loads yaml from file.
	YAML::Node load_file(const xw::string& base_dir, const std::string& file_name) override;

	void overwrite_config(YAML::Node& config, const YAML::Node& local_config) override;

	void init_settings(Settings* settings, const YAML::Node& config) override;
};

__CONF_END__
