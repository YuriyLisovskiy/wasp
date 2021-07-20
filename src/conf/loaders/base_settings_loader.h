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

template <typename ConfigT>
class BaseSettingsLoader
{
protected:
	std::filesystem::directory_entry find_file(
		const std::string& base_dir, const std::string& file_name
	)
	{
		std::regex file_name_regex(file_name);
		auto dir_sep = std::string(1, path::sep);
		for (const auto& entry : std::filesystem::directory_iterator(base_dir))
		{
			if (!entry.is_directory())
			{
				auto str_path = entry.path().string();
				std::string curr_file_name(str_path.begin() + base_dir.size(), str_path.end());
				curr_file_name = str::ltrim(curr_file_name, dir_sep);
				if (std::regex_match(curr_file_name, file_name_regex))
				{
					return entry;
				}
			}
		}

		return std::filesystem::directory_entry();
	}

public:
	[[nodiscard]]
	virtual std::string config_name() const = 0;

	[[nodiscard]]
	virtual std::string local_config_name() const = 0;

	// Reads and loads config from file.
	virtual ConfigT load_file(const std::string& base_dir, const std::string& file_name) = 0;

	virtual void overwrite_config(ConfigT& config, const ConfigT& local_config) = 0;

	virtual void init_settings(Settings* settings, const ConfigT& config) = 0;
};

__CONF_END__
