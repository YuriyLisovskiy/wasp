/**
 * conf/settings.cpp
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 */

#include "./settings.h"

// Render libraries.
#include <xalwart.render/engine.h>
#include <xalwart.render/loaders.h>

// Framework modules.
#include "../render/default.h"
#include "../middleware/clickjacking.h"
#include "../middleware/common.h"
#include "../middleware/cookies.h"
#include "../middleware/http.h"
#include "../middleware/security.h"


__CONF_BEGIN__

Settings::Settings(const std::string& base_dir)
{
	this->BASE_DIR = base_dir;
	this->_middleware = {
		{middleware::XFrameOptions::FULL_NAME, [this]() -> std::shared_ptr<middleware::IMiddleware> {
			return std::make_shared<middleware::XFrameOptions>(this);
		}},
		{middleware::Common::FULL_NAME, [this]() -> std::shared_ptr<middleware::IMiddleware> {
			return std::make_shared<middleware::Common>(this);
		}},
		{middleware::Cookie::FULL_NAME, [this]() -> std::shared_ptr<middleware::IMiddleware> {
			return std::make_shared<middleware::Cookie>(this);
		}},
		{middleware::ConditionalGet::FULL_NAME, [this]() -> std::shared_ptr<middleware::IMiddleware> {
			return std::make_shared<middleware::ConditionalGet>(this);
		}},
		{middleware::Security::FULL_NAME, [this]() -> std::shared_ptr<middleware::IMiddleware> {
			return std::make_shared<middleware::Security>(this);
		}}
	};

	this->_libraries = {
		{render::DefaultLibrary::FULL_NAME, [this]() -> std::shared_ptr<render::abc::ILibrary> {
			return std::make_shared<render::DefaultLibrary>(this);
		}}
	};

	this->_loaders = {
		{render::DefaultLoader::FULL_NAME, []() -> std::shared_ptr<render::DefaultLoader> {
			return std::make_shared<render::DefaultLoader>();
		}}
	};
}

void Settings::prepare()
{
	if (!this->DB)
	{
		this->LOGGER->warning("Missing 'default' database");
		if (!this->DATABASES.empty())
		{
			this->DB = this->DATABASES.begin()->second;
			this->LOGGER->warning("Using the first database from 'databases' map");
		}
	}
}

void Settings::perform_checks()
{
	std::cout << "Performing checks..." << std::endl;
	if (!this->LOGGER)
	{
		throw ImproperlyConfigured("'LOGGER' must be configured", _ERROR_DETAILS_);
	}

	size_t err_count = 0;
	if (this->BASE_DIR.empty())
	{
		this->LOGGER->error("'BASE_DIR' must not be empty in order to use the application.");
		err_count++;
	}
	else if (!path::exists(this->BASE_DIR))
	{
		this->LOGGER->error("'BASE_DIR' must exist in order to use the application.");
		err_count++;
	}

	if (this->SECRET_KEY.empty())
	{
		this->LOGGER->error("'SECRET_KEY' must be set in order to use the application.");
		err_count++;
	}

	if (!this->DB)
	{
		this->LOGGER->error(
			"No database was set, at least one database must be configured in order to use the application."
		);
		err_count++;
	}

	if (!this->TEMPLATE_ENGINE)
	{
		this->LOGGER->error("'TEMPLATE_ENGINE' must be configured in order to use the application.");
		err_count++;
	}

	if (this->MIDDLEWARE.empty())
	{
		this->LOGGER->warning("You have not added any middleware.");
	}

	if (this->MODULES.empty())
	{
		this->LOGGER->warning("You have not added any module to 'modules' setting.");
	}

	for (auto& module : this->MODULES)
	{
		if (!module->ready())
		{
			this->LOGGER->error("Module '" + module->get_name() + "' is not ready.");
			err_count++;
			break;
		}
	}

	if (err_count > 0)
	{
		throw ImproperlyConfigured(
			"System check identified " + std::to_string(err_count) + " issues.", _ERROR_DETAILS_
		);
	}

	std::cout << "Done." << std::endl;
}

std::shared_ptr<IModuleConfig> Settings::get_module(const std::string& full_name) const
{
	if (this->_modules.find(full_name) != this->_modules.end())
	{
		return this->_modules.at(full_name)();
	}

	return nullptr;
}

std::shared_ptr<middleware::IMiddleware> Settings::get_middleware(const std::string& full_name) const
{
	if (this->_middleware.find(full_name) != this->_middleware.end())
	{
		return this->_middleware.at(full_name)();
	}

	return nullptr;
}

std::shared_ptr<render::abc::ILibrary> Settings::get_library(const std::string& full_name) const
{
	if (this->_libraries.find(full_name) != this->_libraries.end())
	{
		return this->_libraries.at(full_name)();
	}

	return nullptr;
}

std::shared_ptr<render::abc::ILoader> Settings::get_loader(const std::string& full_name) const
{
	if (this->_loaders.find(full_name) != this->_loaders.end())
	{
		return this->_loaders.at(full_name)();
	}

	return nullptr;
}

__CONF_END__
