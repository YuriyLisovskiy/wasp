/**
 * conf/loaders/base_settings_loader.h
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 *
 * Purpose: TODO
 */

#pragma once

// C++ libraries.
#include <string>
#include <filesystem>
#include <regex>

// Base libraries.
#include <xalwart.base/path.h>
#include <xalwart.base/string_utils.h>
#include <xalwart.base/exceptions.h>

// Module definitions.
#include "../_def_.h"

// Framework libraries.
#include "../settings.h"


__CONF_BEGIN__

template <typename T>
concept settings_type = std::is_base_of_v<Settings, T>;

template <typename ConfigType>
class BaseSettingsLoader
{
public:
	template<settings_type SettingsType, typename ...Args>
	inline SettingsType load(Args&& ...args)
	{
		SettingsType settings(std::forward<Args>(args)...);
		auto config = this->load_file(settings.BASE_DIR, this->config_name());
		if (!config)
		{
			throw RuntimeError("'" + this->config_name() + "' is not found");
		}

		auto local_config = this->load_file(settings.BASE_DIR, this->local_config_name());
		this->overwrite_config(config, local_config);
		this->init_settings(&settings, config);
		return settings;
	}

protected:
	[[nodiscard]]
	virtual std::string config_name() const = 0;

	[[nodiscard]]
	virtual std::string local_config_name() const = 0;

	// Reads and loads config from file.
	virtual ConfigType load_file(const std::string& base_dir, const std::string& file_name) = 0;

	virtual void overwrite_config(ConfigType& config, const ConfigType& local_config) = 0;

	virtual void init_settings(Settings* settings, const ConfigType& config) = 0;

	inline std::filesystem::directory_entry find_file(const std::string& base_dir, const std::string& file_name)
	{
		std::regex file_name_regex(file_name);
		auto dir_sep = std::string(1, path::path_sep);
		for (const auto& path_entry : std::filesystem::directory_iterator(base_dir))
		{
			if (!path_entry.is_directory())
			{
				auto current_file_name = path_entry.path().string().substr(base_dir.size());
				current_file_name = str::ltrim(current_file_name, dir_sep);
				if (std::regex_match(current_file_name, file_name_regex))
				{
					return path_entry;
				}
			}
		}

		return {};
	}
};

__CONF_END__
