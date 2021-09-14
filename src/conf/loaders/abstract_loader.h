/**
 * conf/loaders/abstract_loader.h
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 *
 * Purpose: TODO
 */

#pragma once

// C++ libraries.
#include <string>
#include <memory>
#include <filesystem>
#include <regex>
#include <functional>

// Base libraries.
#include <xalwart.base/path.h>
#include <xalwart.base/string_utils.h>
#include <xalwart.base/exceptions.h>
#include <xalwart.base/config/component.h>

// Module definitions.
#include "../_def_.h"

// Framework libraries.
#include "../settings.h"


__CONF_BEGIN__

template <typename T>
concept settings_type = std::is_base_of_v<Settings, T>;

template <typename ConfigType, settings_type SettingsType>
class AbstractSettingsLoader
{
public:
	explicit AbstractSettingsLoader(std::string file_extension_pattern) :
		_file_extension_pattern(std::move(file_extension_pattern))
	{
	}

	template<typename ...SettingsArgs>
	inline std::unique_ptr<SettingsType> load(SettingsArgs&& ...args)
	{
		auto settings = std::make_unique<SettingsType>(std::forward<SettingsArgs>(args)...);
		auto config = this->load_file(
			settings->BASE_DIR,
			std::regex(this->configuration_name_regex() + R"(\.)" + this->extension_pattern())
		);
		if (!config)
		{
			throw RuntimeError("'" + this->configuration_name_regex() + "' is not found");
		}

		this->initialize_components(settings.get());

		auto local_config = this->load_file(
			settings->BASE_DIR,
			std::regex(this->local_configuration_name_regex() + R"(\.)" + this->extension_pattern())
		);
		if (local_config)
		{
			for (const auto& component : this->components)
			{
				auto to_component = config[component.first];
				component.second->overwrite(local_config[component.first], to_component);
				config[component.first] = to_component;
			}
		}

		for (const auto& component : this->components)
		{
			component.second->initialize(config[component.first]);
		}

		return settings;
	}

	inline void register_component(std::string name, std::unique_ptr<config::AbstractComponent<ConfigType>> loader)
	{
		this->components.template emplace_back(std::move(name), std::move(loader));
	}

protected:
	std::list<std::pair<std::string, std::unique_ptr<config::AbstractComponent<ConfigType>>>> components{};

	virtual void initialize_components(SettingsType* settings) = 0;

	[[nodiscard]]
	inline std::string extension_pattern() const
	{
		return this->_file_extension_pattern;
	}

	[[nodiscard]]
	virtual std::string configuration_name_regex() const = 0;

	[[nodiscard]]
	virtual std::string local_configuration_name_regex() const = 0;

	// Reads and loads config from file.
	virtual ConfigType load_file(const std::string& base_dir, const std::regex& file_name_pattern) = 0;

	inline std::filesystem::directory_entry find_file(const std::string& base_dir, const std::regex& file_name_pattern)
	{
		auto dir_sep = std::string(1, path::path_sep);
		for (const auto& path_entry : std::filesystem::directory_iterator(base_dir))
		{
			if (!path_entry.is_directory())
			{
				auto current_file_name = path_entry.path().string().substr(base_dir.size());
				current_file_name = str::ltrim(current_file_name, dir_sep);
				if (std::regex_match(current_file_name, file_name_pattern))
				{
					return path_entry;
				}
			}
		}

		return {};
	}

private:
	std::string _file_extension_pattern;
};

__CONF_END__
