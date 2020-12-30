/**
 * conf/settings_factory.h
 *
 * Copyright (c) 2020 Yuriy Lisovskiy
 *
 * Purpose: TODO
 */

#pragma once

//// C++ libraries.
//#include <map>
//#include <memory>
//#include <functional>
//
//// Core libraries.
//#include <xalwart.core/logger.h>
//
//// Module definitions.
//#include "./_def_.h"
//
//// Framework libraries.
////#include "../apps/interfaces.h"
////#include "../middleware/interfaces.h"
////#include "../render/library/library.h"
//#include "../render/library/library.h"
//#include "../apps/_def_.h"
//#include "../middleware/_def_.h"
//
//#include <xalwart.render/library/_def_.h>
//
////#include "./settings.h"
//
//__CONF_BEGIN__
//struct Settings;
//__CONF_END__
//
//__MIDDLEWARE_BEGIN__
//class IMiddleware;
//__MIDDLEWARE_END__
//
//__LIB_BEGIN__
//class ILibrary;
//__LIB_END__
//
//__APPS_BEGIN__
//class IAppConfig;
//__APPS_END__
//
//__RENDER_BEGIN__
//class ILoader;
//__RENDER_END__
//
//
//__CONF_BEGIN__
//
//template <class>
//class SettingsFactory final
//{
//private:
//	std::map<
//		std::string,
//		std::function<std::shared_ptr<middleware::IMiddleware>()>
//	> _middleware;
//
//	std::map<
//		std::string,
//		std::function<std::shared_ptr<render::lib::ILibrary>()>
//	> _libraries;
//
//	std::map<
//		std::string,
//		std::function<std::shared_ptr<apps::IAppConfig>()>
//	> _apps;
//
//	std::map<
//		std::string,
//		std::function<std::shared_ptr<render::ILoader>()>
//	> _loaders;
//
//	Settings* _settings;
//	core::ILogger* _logger;
//
//public:
//	SettingsFactory(nullptr_t) = delete;
//	SettingsFactory(core::ILogger* logger);
//
//	template <typename T, typename = std::enable_if<std::is_base_of<apps::IAppConfig, T>::value>>
//	void register_app(const std::string& full_name)
//	{
//		if (this->_apps.find(full_name) != this->_apps.end())
//		{
//			if (this->_logger)
//			{
//				this->_logger->warning(
//					"unable to register '" + full_name + "' app which already exists"
//				);
//			}
//		}
//		else
//		{
//			this->_apps[full_name] = [this]() -> std::shared_ptr<apps::IAppConfig> {
//				auto app = std::make_shared<T>(this->_settings);
//				app->init(app->__type__());
//				return app;
//			};
//		}
//	}
//
//	template <typename T, typename = std::enable_if<std::is_base_of<middleware::IMiddleware, T>::value>>
//	void register_middleware(const std::string& full_name)
//	{
//		if (this->_middleware.find(full_name) != this->_middleware.end())
//		{
//			if (this->_logger)
//			{
//				this->_logger->warning(
//					"unable to register '" + full_name + "' middleware which already exists"
//				);
//			}
//		}
//		else
//		{
//			this->_middleware[full_name] = [this]() -> std::shared_ptr<middleware::IMiddleware> {
//				return std::make_shared<T>(this->_settings);
//			};
//		}
//	}
//
//	template <typename T, typename = std::enable_if<std::is_base_of<render::lib::Library, T>::value>>
//	void register_library(const std::string& full_name)
//	{
//		if (this->_libraries.find(full_name) != this->_libraries.end())
//		{
//			if (this->_logger)
//			{
//				this->_logger->warning(
//					"unable to register '" + full_name + "' library which already exists"
//				);
//			}
//		}
//		else
//		{
//			this->_libraries[full_name] = [this]() -> std::shared_ptr<render::lib::ILibrary> {
//				return std::make_shared<T>(this->_settings);
//			};
//		}
//	}
//
//	template <typename T, typename = std::enable_if<std::is_base_of<render::ILoader, T>::value>>
//	void register_loader(const std::string& full_name)
//	{
//		if (this->_loaders.find(full_name) != this->_loaders.end())
//		{
//			if (this->_logger)
//			{
//				this->_logger->warning(
//					"unable to register '" + full_name + "' loader which already exists"
//				);
//			}
//		}
//		else
//		{
//			this->_loaders[full_name] = [this]() -> std::shared_ptr<render::ILoader> {
//				return std::make_shared<T>(this->_settings);
//			};
//		}
//	}
//
//	[[nodiscard]]
//	std::shared_ptr<apps::IAppConfig> get_app(const std::string& full_name) const;
//
//	[[nodiscard]]
//	std::shared_ptr<middleware::IMiddleware> get_middleware(const std::string& full_name) const;
//
//	[[nodiscard]]
//	std::shared_ptr<render::lib::ILibrary> get_library(const std::string& full_name) const;
//
//	[[nodiscard]]
//	std::shared_ptr<render::ILoader> get_loader(const std::string& full_name) const;
//};
//
//__CONF_END__
