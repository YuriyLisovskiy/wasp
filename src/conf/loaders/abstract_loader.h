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
#include <vector>

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
	// `filename_patterns` is a list of filenames regular expressions without an
	// extension which will be read and the previous one will be overwritten by the
	// next in the list.
	//
	// Example:
	//  filename_patterns = {"config", R"(config\.dev)", "config.prod"}
	//  '->' means A overwrites B:
	//  config.prod.yaml -> config.dev.yaml -> config.yaml
	explicit AbstractSettingsLoader(
		std::string file_extension_pattern, std::vector<std::string> filename_patterns
	) : _file_extension_pattern(std::move(file_extension_pattern)), filename_patterns(std::move(filename_patterns))
	{
	}

	template<typename ...SettingsArgs>
	inline std::unique_ptr<SettingsType> load(SettingsArgs&& ...args)
	{
		auto settings = std::make_unique<SettingsType>(std::forward<SettingsArgs>(args)...);
		if (!settings->BASE_DIR.exists())
		{
			throw ImproperlyConfigured(
				"'BASE_DIR' must exist in order to use the application.", _ERROR_DETAILS_
			);
		}

		if (!this->filename_patterns.empty())
		{
			auto config_pattern = this->filename_patterns.front() + R"(\.)" + this->extension_pattern();
			auto main_config = this->load_file(settings->BASE_DIR.to_string(), std::regex(config_pattern));
			if (!main_config)
			{
				throw RuntimeError("Main configuration file with pattern '" + config_pattern + "' is not found.");
			}

			settings->register_components();
			this->initialize_components(settings.get());

			auto filenames_count = this->filename_patterns.size();
			for (size_t i = 1; i < filenames_count; i++)
			{
				auto pattern = this->filename_patterns[i] + R"(\.)" + this->extension_pattern();
				auto next_config = this->load_file(settings->BASE_DIR.to_string(), std::regex(pattern));
				if (next_config)
				{
					for (const auto& component : this->components)
					{
						auto to_component = main_config[component.first];
						component.second->overwrite(next_config[component.first], to_component);
						main_config[component.first] = to_component;
					}
				}
			}

			for (const auto& component : this->components)
			{
				component.second->initialize(main_config[component.first]);
			}
		}

		return settings;
	}

	inline void register_component(
		std::string name, std::unique_ptr<config::AbstractComponent<ConfigType>> component
	)
	{
		this->components.template emplace_back(std::move(name), std::move(component));
	}

protected:
	std::vector<std::string> filename_patterns;
	std::list<std::pair<std::string, std::unique_ptr<config::AbstractComponent<ConfigType>>>> components{};

	virtual void initialize_components(SettingsType* settings) = 0;

	// Reads and loads config from file.
	// Should return `nullptr` if file is not found.
	virtual ConfigType load_file(const std::string& base_dir, const std::regex& file_name_pattern) = 0;

	[[nodiscard]]
	inline std::string extension_pattern() const
	{
		return this->_file_extension_pattern;
	}

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
