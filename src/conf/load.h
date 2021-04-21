/**
 * conf/load.h
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 *
 * Purpose: TODO
 */

#pragma once

// C++ libraries.
#include <memory>

// Core libraries.
#include <xalwart.core/exceptions.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "./loaders/base_settings_loader.h"


__CONF_BEGIN__

template <typename T>
concept SettingsType = std::is_base_of_v<Settings, T>;

template <typename SettingLoaderT>
struct loader
{
private:
	std::shared_ptr<SettingLoaderT> _loader;

public:
	explicit loader(std::shared_ptr<SettingLoaderT> l) : _loader(l)
	{
		if (!this->_loader)
		{
			throw NullPointerException(
				"settings loader is not instantiated", _ERROR_DETAILS_
			);
		}
	}

	template<SettingsType T, typename ...Args>
	std::shared_ptr<T> load_settings(Args&& ...args)
	{
		auto settings = std::make_shared<T>(std::forward<Args>(args)...);

		auto config = this->_loader->load_file(settings->BASE_DIR, this->_loader->config_name());
		if (!config)
		{
			throw RuntimeError("'" + this->_loader->config_name() + "' is not found");
		}

		auto local_config = this->_loader->load_file(
			settings->BASE_DIR, this->_loader->local_config_name()
		);

		this->_loader->overwrite_config(config, local_config);

		this->_loader->init_settings(settings.get(), config);
		return settings;
	}
};

template <typename SettingLoaderT, typename ...Args>
loader<SettingLoaderT> with_loader(Args&& ...args)
{
	return loader<SettingLoaderT>(
		std::make_shared<SettingLoaderT>(std::forward<Args>(args)...)
	);
}

__CONF_END__
