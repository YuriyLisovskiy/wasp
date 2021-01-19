/**
 * conf/loaders/yaml_settings_loader.h
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 *
 * Purpose: TODO
 */

#pragma once

// C++ libraries.
#include <iostream>

// Core libraries.
#include <xalwart.core/path.h>
#include <xalwart.core/yaml/yaml-cpp/yaml.h>

// Module definitions.
#include "../_def_.h"

// Framework libraries.
#include "../settings.h"


__CONF_BEGIN__

template <typename SettingsT>
concept SettingsType = std::is_base_of_v<Settings, SettingsT>;

class YamlSettingsLoader
{
private:
	void _init_allowed_hosts(Settings* settings, const YAML::Node& allowed_hosts);

	void _init_disallowed_user_agents(Settings* settings, const YAML::Node& agents);

	void _init_ignorable_404_urls(Settings* settings, const YAML::Node& urls);

	void _init_formats(Settings* settings, const YAML::Node& config);

	void _init_csrf(Settings* settings, const YAML::Node& config);

	void _init_secure(Settings* settings, const YAML::Node& config);

	void _init_apps(Settings* settings, const YAML::Node& apps);

	void _init_middleware(Settings* settings, const YAML::Node& middleware);

protected:

	// Override this method to change the default config name.
	[[nodiscard]]
	virtual std::string config_name() const;

	// Override this method to change the default local config name.
	[[nodiscard]]
	virtual std::string local_config_name() const;

	// Returns YAML::Node of type Null
	[[nodiscard]]
	YAML::Node null() const;

	// Returns YAML::Node of type Map
	[[nodiscard]]
	YAML::Node map_node() const;

	// Reads and loads yaml from file.
	virtual YAML::Node load_yaml(const std::string& file_path);

	virtual void check_config(const YAML::Node& config, const std::string& file_path);

	void overwrite_scalar_or_remove_if_null(
		YAML::Node& config, const YAML::Node& local_config, const std::string& key
	);
	void overwrite_sequence_or_remove_if_null(
		YAML::Node& config, const YAML::Node& local_config, const std::string& key
	);

	virtual void overwrite_config(YAML::Node& config, const YAML::Node& local_config);

	// Override this method to setup custom logger with custom parameters.
	virtual void overwrite_logger(YAML::Node& logger_cfg, const YAML::Node& local_logger_cfg);

	// Override this method to setup custom render engine with custom parameters.
	virtual void overwrite_template_engine(
		YAML::Node& engine_cfg, const YAML::Node& local_engine_cfg
	);

	virtual void init_settings(Settings* settings, const YAML::Node& config);

	virtual void init_logger_setting(Settings* settings, const YAML::Node& logger_config);

	virtual void init_template_engine_setting(Settings* settings, const YAML::Node& engine_config);

public:
	template<SettingsType T, typename ...Args>
	std::shared_ptr<T> load(Args&& ...args)
	{
		auto settings = std::make_shared<T>(std::forward<Args>(args)...);

		std::string main_path = path::join(settings->BASE_DIR, this->config_name());
		auto config = this->load_yaml(main_path);
		if (!config)
		{
			throw core::RuntimeError("'" + this->config_name() + "' is not found");
		}

		std::string local_path = path::join(settings->BASE_DIR, this->local_config_name());
		auto local_config = this->load_yaml(local_path);

		if (local_config && !local_config.IsNull())
		{
			this->overwrite_config(config, local_config);
		}

		this->init_settings(settings.get(), config);
		return settings;
	}
};

__CONF_END__
