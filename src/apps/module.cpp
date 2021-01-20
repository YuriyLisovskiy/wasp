/**
 * apps/module.cpp
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 */

#include "./module.h"


__APPS_BEGIN__

ModuleConfig::ModuleConfig(
	std::string module_path, conf::Settings* settings
) : _is_initialized(false), settings(settings), module_path(std::move(module_path))
{
}

void ModuleConfig::urlpatterns()
{
}

void ModuleConfig::commands()
{
}

bool ModuleConfig::ready() const
{
	return this->_is_initialized;
}

std::string ModuleConfig::get_name() const
{
	return this->module_name;
}

std::string ModuleConfig::get_module_path() const
{
	return this->module_path;
}

std::vector<std::shared_ptr<urls::UrlPattern>> ModuleConfig::get_urlpatterns()
{
	if (this->_urlpatterns.empty())
	{
		this->urlpatterns();
		for (auto& module_to_init : this->_sub_modules_to_init)
		{
			module_to_init();
		}
	}

	return this->_urlpatterns;
}

std::vector<std::shared_ptr<cmd::BaseCommand>> ModuleConfig::get_commands()
{
	if (this->_commands.empty())
	{
		this->commands();
	}

	return this->_commands;
}

void ModuleConfig::init(const std::string& name)
{
	if (name.empty())
	{
		throw core::ValueError("module name can not be empty", _ERROR_DETAILS_);
	}

	if (!this->_is_initialized)
	{
		this->module_name = name;
		this->_is_initialized = true;
	}
}

__APPS_END__
