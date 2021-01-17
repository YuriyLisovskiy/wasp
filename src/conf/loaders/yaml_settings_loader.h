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
protected:
	[[nodiscard]]
	virtual std::string config_name() const;

	[[nodiscard]]
	virtual std::string local_config_name() const;

	YAML::Node null() const;

	YAML::Node undefined() const;

	YAML::Node map_node() const;

	YAML::Node get_or_null(const YAML::Node& node) const;

	YAML::Node get_or_undefined(const YAML::Node& node) const;

	virtual YAML::Node load_yaml(const std::string& file_path);

	virtual void check_config(const YAML::Node& config, const std::string& file_path);

	// Loads local configuration file and overrides existing one.
	void overwrite_scalar_or_remove_if_null(
		YAML::Node& config, const YAML::Node& local_config, const std::string& key
	);
	void overwrite_sequence_or_remove_if_null(
		YAML::Node& config, const YAML::Node& local_config, const std::string& key
	);

	virtual void overwrite_config(YAML::Node& config, const YAML::Node& local_config);

	virtual void overwrite_logger(YAML::Node& logger_cfg, const YAML::Node& local_logger_cfg);

	virtual void overwrite_template_engine(
		YAML::Node& engine_cfg, const YAML::Node& local_engine_cfg
	);

	virtual void init(Settings* settings, const YAML::Node& config);

	virtual void init_logger(Settings* settings, const YAML::Node& logger_config);

	virtual void init_allowed_hosts(Settings* settings, const YAML::Node& allowed_hosts);

	virtual void init_disallowed_user_agents(Settings* settings, const YAML::Node& agents);

	virtual void init_ignorable_404_urls(Settings* settings, const YAML::Node& urls);

	virtual void init_formats(Settings* settings, const YAML::Node& config);

	virtual void init_csrf(Settings* settings, const YAML::Node& config);

	virtual void init_secure(Settings* settings, const YAML::Node& config);

	virtual void init_apps(Settings* settings, const YAML::Node& apps);

	virtual void init_middleware(Settings* settings, const YAML::Node& middleware);

	virtual void init_template_engine(Settings* settings, const YAML::Node& engine_config);

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

		if (local_config)
		{
			if (config)
			{
				this->overwrite_config(config, local_config);
			}
			else
			{
				config = local_config;
			}
		}

		if (config)
		{
			this->init(settings.get(), config);
		}

		return settings;
	}
};

__CONF_END__
