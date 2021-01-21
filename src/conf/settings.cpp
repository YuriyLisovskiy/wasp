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
#include "../render/library/default.h"
#include "../middleware/clickjacking.h"
#include "../middleware/common.h"
#include "../middleware/cookies.h"
#include "../middleware/http.h"
#include "../middleware/security.h"


__CONF_BEGIN__

Settings::Settings(const std::string& base_dir)
{
	this->BASE_DIR = base_dir;
	using namespace middleware;
	this->_middleware = {
		{XFrameOptionsMiddleware::FULL_NAME, [this]() -> std::shared_ptr<middleware::IMiddleware> {
			return std::make_shared<XFrameOptionsMiddleware>(this);
		}},
		{CommonMiddleware::FULL_NAME, [this]() -> std::shared_ptr<middleware::IMiddleware> {
			return std::make_shared<CommonMiddleware>(this);
		}},
		{CookieMiddleware::FULL_NAME, [this]() -> std::shared_ptr<middleware::IMiddleware> {
			return std::make_shared<CookieMiddleware>(this);
		}},
		{ConditionalGetMiddleware::FULL_NAME, [this]() -> std::shared_ptr<middleware::IMiddleware> {
			return std::make_shared<ConditionalGetMiddleware>(this);
		}},
		{SecurityMiddleware::FULL_NAME, [this]() -> std::shared_ptr<middleware::IMiddleware> {
			return std::make_shared<SecurityMiddleware>(this);
		}}
	};

	this->_libraries = {
		{render::DefaultLibrary::FULL_NAME, [this]() -> std::shared_ptr<render::ILibrary> {
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
	if (!this->ROOT_MODULE && !this->INSTALLED_MODULES.empty())
	{
		this->ROOT_MODULE = this->INSTALLED_MODULES.front();
	}
}

void Settings::register_modules()
{
}

void Settings::register_middleware()
{
}

void Settings::register_libraries()
{
}

void Settings::register_loaders()
{
}

std::shared_ptr<apps::IModuleConfig> Settings::get_module(
	const std::string& full_name
) const
{
	if (this->_modules.find(full_name) != this->_modules.end())
	{
		return this->_modules.at(full_name)();
	}

	return nullptr;
}

std::shared_ptr<middleware::IMiddleware> Settings::get_middleware(
	const std::string& full_name
) const
{
	if (this->_middleware.find(full_name) != this->_middleware.end())
	{
		return this->_middleware.at(full_name)();
	}

	return nullptr;
}

std::shared_ptr<render::ILibrary> Settings::get_library(
	const std::string& full_name
) const
{
	if (this->_libraries.find(full_name) != this->_libraries.end())
	{
		return this->_libraries.at(full_name)();
	}

	return nullptr;
}

std::shared_ptr<render::ILoader> Settings::get_loader(
	const std::string& full_name
) const
{
	if (this->_loaders.find(full_name) != this->_loaders.end())
	{
		return this->_loaders.at(full_name)();
	}

	return nullptr;
}

__CONF_END__
