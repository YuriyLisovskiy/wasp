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

// Base libraries.
#include <xalwart.base/exceptions.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "./loaders/base_settings_loader.h"


__CONF_BEGIN__

template <typename T>
concept settings_type = std::is_base_of_v<Settings, T>;

// TESTME: Loader
// TODO: docs for 'Loader<SettingLoaderT>'
template <typename SettingLoaderT>
struct Loader
{
private:
	std::shared_ptr<SettingLoaderT> _loader;

public:
	explicit Loader(std::shared_ptr<SettingLoaderT> l) : _loader(l)
	{
		if (!this->_loader)
		{
			throw NullPointerException("settings loader is not instantiated", _ERROR_DETAILS_);
		}
	}

	template<settings_type T, typename ...Args>
	inline std::shared_ptr<T> load_settings(Args&& ...args)
	{
		auto settings = std::make_shared<T>(std::forward<Args>(args)...);
		auto config = this->_loader->load_file(settings->BASE_DIR, this->_loader->config_name());
		if (!config)
		{
			throw RuntimeError("'" + this->_loader->config_name() + "' is not found");
		}

		auto local_config = this->_loader->load_file(settings->BASE_DIR, this->_loader->local_config_name());
		this->_loader->overwrite_config(config, local_config);
		this->_loader->init_settings(settings.get(), config);
		return settings;
	}
};

// TESTME: with_loader<SettingLoaderT, ...ArgsT>
// TODO: docs for 'with_loader<SettingLoaderT, ...ArgsT>'
template <typename SettingLoaderT, typename ...ArgsT>
inline Loader<SettingLoaderT> with_loader(ArgsT&& ...args)
{
	return Loader<SettingLoaderT>(std::make_shared<SettingLoaderT>(std::forward<ArgsT>(args)...));
}

__CONF_END__
