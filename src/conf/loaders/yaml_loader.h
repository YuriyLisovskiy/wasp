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
#include <functional>

// Base libraries.
#include <xalwart.base/yaml/yaml-cpp/yaml.h>
#include <xalwart.base/config/components/yaml/default.h>
#include <xalwart.base/config/components/yaml/logger.h>

// Module definitions.
#include "../_def_.h"

// Framework libraries.
#include "./abstract_loader.h"
#include "./yaml/csrf.h"
#include "./yaml/formats.h"
#include "./yaml/limits.h"
#include "./yaml/secure.h"
#include "./yaml/static.h"
#include "./yaml/timezone.h"


__CONF_BEGIN__

template <settings_type SettingsType>
class YAMLSettingsLoader : public AbstractSettingsLoader<YAML::Node, SettingsType>
{
public:
	inline explicit YAMLSettingsLoader(
		const std::vector<std::string>& filename_patterns={"config", R"(config\.dev)"}
	) : AbstractSettingsLoader<YAML::Node, SettingsType>(R"(y(a)?ml)", filename_patterns)
	{
	}

	inline void register_default_logger(SettingsType* settings)
	{
		this->register_component("logger", std::make_unique<config::YAMLLoggerComponent>(
			settings->BASE_DIR, settings->LOGGER
		));
	}

	inline void register_standard_components(SettingsType* settings)
	{
		this->register_component("debug", std::make_unique<config::YAMLScalarComponent>(settings->DEBUG));
		this->register_component("secret_key", std::make_unique<config::YAMLScalarComponent>(settings->SECRET_KEY));
		this->register_component(
			"allowed_hosts", std::make_unique<config::YAMLSequenceComponent>([settings](const YAML::Node& host)
			{
				auto value = host.as<std::string>("");
			    if (!value.empty())
			    {
					settings->ALLOWED_HOSTS.push_back(value);
			    }
			})
		);
		this->register_component(
			"use_timezone", std::make_unique<config::YAMLScalarComponent>(settings->USE_TIMEZONE)
		);
		this->register_component("timezone", std::make_unique<YAMLTimezoneComponent>(settings->TIMEZONE));
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
				auto regular_expression = user_agent.as<std::string>("");
				if (!regular_expression.empty())
				{
					settings->DISALLOWED_USER_AGENTS.emplace_back(regular_expression);
				}
			})
		);
		this->register_component(
			"ignorable_404_urls",
			std::make_unique<config::YAMLSequenceComponent>([settings](const YAML::Node& url)
			{
				auto regular_expression = url.as<std::string>("");
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
		this->register_component("formats", std::make_unique<YAMLFormatsComponent>(settings->FORMATS));
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
		this->register_component("csrf", std::make_unique<YAMLCSRFComponent>(settings->CSRF));
		this->register_component("use_ssl", std::make_unique<config::YAMLScalarComponent>(settings->USE_SSL));
		this->register_component("secure", std::make_unique<YAMLSecureComponent>(settings->SECURE));
		this->register_component(
			"modules", std::make_unique<config::YAMLSequenceComponent>([settings](const YAML::Node& node)
			{
				auto module_name = node.as<std::string>("");
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
		this->register_component(
			"middleware", std::make_unique<config::YAMLSequenceComponent>([settings](const YAML::Node& node)
			{
				auto middleware_name = node.as<std::string>("");
				if (!middleware_name.empty())
				{
					auto middleware = settings->get_middleware_by_name(middleware_name);
					if (middleware)
					{
						settings->MIDDLEWARE.push_back(middleware);
					}
					else if (settings->LOGGER)
					{
						settings->LOGGER->warning("Middleware not registered: '" + middleware_name + "'");
					}
				}
			})
		);
	}

	inline YAMLSettingsLoader<SettingsType>& with_components(
		std::function<void(YAMLSettingsLoader<SettingsType>*, SettingsType*)> function
	)
	{
		if (function)
		{
			this->initializer = std::move(function);
		}

		return *this;
	}

protected:
	std::vector<std::string> configuration_files;
	std::function<void(YAMLSettingsLoader<SettingsType>*, SettingsType*)> initializer;

	inline void initialize_components(SettingsType* settings) override
	{
		if (this->initializer)
		{
			this->initializer(this, settings);
		}
	}

	virtual inline void check_config(const YAML::Node& config, const std::string& file_path)
	{
		if (config && config.IsMap())
		{
			return;
		}

		throw ValueError(
			"Configuration file with pattern '" + file_path + "' should have map type", _ERROR_DETAILS_
		);
	}

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
};

__CONF_END__
