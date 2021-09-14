/**
 * conf/loaders/yaml_loader.h
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 *
 * Purpose: TODO
 */

#pragma once

// C++ libraries.
#include <regex>

// Base libraries.
#include <xalwart.base/yaml/yaml-cpp/yaml.h>
#include <xalwart.base/config/components/yaml/default.h>
#include <xalwart.base/config/components/yaml/logger.h>

// Module definitions.
#include "../_def_.h"

// Framework libraries.
#include "./abstract_loader.h"
#include "./yaml/limits.h"
#include "./yaml/static.h"


__CONF_BEGIN__

template <settings_type SettingsType>
class YAMLSettingsLoader : public AbstractSettingsLoader<YAML::Node, SettingsType>
{
public:
	inline YAMLSettingsLoader() : AbstractSettingsLoader<YAML::Node, SettingsType>(R"(y(a)?ml)")
	{
	}

protected:
	inline void initialize_components(SettingsType* settings) override
	{
		this->register_component("debug", std::make_unique<config::YAMLScalarComponent>(settings->DEBUG));

		this->register_component("logger", std::make_unique<config::YAMLLoggerComponent>(
			settings->BASE_DIR, settings->LOGGER
		));

		this->register_component("secret_key", std::make_unique<config::YAMLScalarComponent>(settings->SECRET_KEY));

		this->register_component(
			"allowed_hosts", std::make_unique<config::YAMLSequenceComponent>([settings](const YAML::Node& host)
			{
				auto value = host.as<std::string>();
			    if (!value.empty())
			    {
					settings->ALLOWED_HOSTS.push_back(value);
			    }
			})
		);

		this->register_component(
			"use_timezone", std::make_unique<config::YAMLScalarComponent>(settings->USE_TIMEZONE)
		);

		// TODO:
		//	this->register_component("timezone", initialize_timezone);

		this->register_component("charset", std::make_unique<config::YAMLScalarComponent>(settings->CHARSET));

		this->register_component(
			"append_slash", std::make_unique<config::YAMLScalarComponent>(
				[settings](const YAML::Node& append_slash)
				{
					settings->APPEND_SLASH = append_slash.as<bool>(settings->APPEND_SLASH);
				}
			)
		);

		this->register_component(
			"disallowed_user_agents",
			std::make_unique<config::YAMLSequenceComponent>([settings](const YAML::Node& user_agent)
			{
				auto regular_expression = user_agent.as<std::string>();
				if (!regular_expression.empty())
				{
					settings->DISALLOWED_USER_AGENTS.emplace_back(regular_expression);
				}
			})
		);

		this->register_component(
			"disallowed_user_agents",
			std::make_unique<config::YAMLSequenceComponent>([settings](const YAML::Node& url)
			{
				auto regular_expression = url.as<std::string>();
				if (!regular_expression.empty())
				{
					settings->IGNORABLE_404_URLS.emplace_back(regular_expression);
				}
			})
		);

		this->register_component(
			"media", std::make_unique<YAMLStaticComponent>(settings->MEDIA, settings->BASE_DIR)
		);

		this->register_component(
			"static", std::make_unique<YAMLStaticComponent>(settings->STATIC, settings->BASE_DIR)
		);

		this->register_component("limits", std::make_unique<YAMLLimitsComponent>(settings->LIMITS));

		this->register_component("prepend_www", std::make_unique<config::YAMLScalarComponent>(settings->PREPEND_WWW));

		// TODO:
	//	auto formats = config["formats"];
	//	if (formats && formats.IsMap())
	//	{
	//		_init_formats(settings, formats);
	//	}

		this->register_component(
			"first_day_of_week", std::make_unique<config::YAMLScalarComponent>(settings->FIRST_DAY_OF_WEEK)
		);

		this->register_component(
			"decimal_separator", std::make_unique<config::YAMLScalarComponent>(settings->DECIMAL_SEPARATOR)
		);

		this->register_component(
			"use_thousand_separator", std::make_unique<config::YAMLScalarComponent>(settings->USE_THOUSAND_SEPARATOR)
		);

		this->register_component(
			"thousand_separator", std::make_unique<config::YAMLScalarComponent>(settings->THOUSAND_SEPARATOR)
		);

		this->register_component(
			"x_frame_options", std::make_unique<config::YAMLScalarComponent>([settings](const YAML::Node& node)
			{
				settings->X_FRAME_OPTIONS = XFrameOptions::from_string(
					node.as<std::string>(settings->X_FRAME_OPTIONS.to_string())
				);
			})
		);

		this->register_component(
			"use_x_forwarded_host", std::make_unique<config::YAMLScalarComponent>(settings->USE_X_FORWARDED_HOST)
		);

		this->register_component(
			"use_x_forwarded_port", std::make_unique<config::YAMLScalarComponent>(settings->USE_X_FORWARDED_PORT)
		);

		// TODO:
	//	auto csrf = config["csrf"];
	//	if (csrf && csrf.IsMap())
	//	{
	//		_init_csrf(settings, csrf);
	//	}

		this->register_component("use_ssl", std::make_unique<config::YAMLScalarComponent>(settings->USE_SSL));

		// TODO:
	//	auto secure = config["secure"];
	//	if (secure && secure.IsMap())
	//	{
	//		_init_secure(settings, secure);
	//	}

		settings->register_modules();
		this->register_component(
			"modules", std::make_unique<config::YAMLSequenceComponent>([settings](const YAML::Node& node)
			{
				auto module_name = node.as<std::string>();
				if (!module_name.empty())
				{
					auto module = settings->build_module(module_name);
					if (module)
					{
						settings->MODULES.push_back(module);
					}
				}
			})
		);

		settings->register_middleware();
		this->register_component(
			"middleware", std::make_unique<config::YAMLSequenceComponent>([settings](const YAML::Node& node)
			{
				auto middleware_name = node.as<std::string>();
				auto middleware = settings->get_middleware_by_name(middleware_name);
				if (middleware)
				{
					settings->MIDDLEWARE.push_back(middleware);
				}
				else if (settings->LOGGER)
				{
					settings->LOGGER->warning("Middleware not registered: '" + middleware_name + "'");
				}
			})
		);
	}

	virtual inline void check_config(const YAML::Node& config, const std::string& file_path)
	{
		if (config && config.IsMap())
		{
			return;
		}

		throw ValueError("'" + file_path + "' file must have map type", _ERROR_DETAILS_);
	}

	// Override this method to change the default config name.
	[[nodiscard]]
	inline std::string configuration_name_regex() const override
	{
		return "config";
	}

	// Override this method to change the default local config name.
	[[nodiscard]]
	inline std::string local_configuration_name_regex() const override
	{
		return R"(config\.local)";
	}

	// Reads and loads yaml from file.
	inline YAML::Node load_file(const std::string& base_dir, const std::regex& file_name_pattern) override
	{
		auto file_entry = this->find_file(base_dir, file_name_pattern);
		if (file_entry.exists())
		{
			auto config = YAML::LoadFile(file_entry.path());
			this->check_config(config, file_entry.path());
			return config;
		}

		return YAML::Node(YAML::NodeType::Null);
	}

//	void overwrite_config(YAML::Node& config, const YAML::Node& local_config) override;
//
//	void init_settings(Settings* settings, const YAML::Node& config) override;

private:

//	static void _init_formats(Settings* settings, const YAML::Node& config);
//
//	static void _init_csrf(Settings* settings, const YAML::Node& config);
//
//	static void _init_secure(Settings* settings, const YAML::Node& config);
//
//	static void _init_modules(Settings* settings, const YAML::Node& modules);
//
//	static void _init_middleware(Settings* settings, const YAML::Node& middleware);
//
//	static void _init_databases(Settings* settings, const YAML::Node& databases);
//
//	static void _init_sqlite3_database(
//		const std::string& name, Settings* settings,
//		std::shared_ptr<orm::abc::ISQLDriver>& driver, const YAML::Node& database
//	);
};

__CONF_END__
