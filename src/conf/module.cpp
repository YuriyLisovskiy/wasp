/**
 * conf/module.cpp
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 */

#include "./module.h"


__CONF_BEGIN__

std::vector<std::shared_ptr<urls::IPattern>> ModuleConfig::get_urlpatterns()
{
	if (this->_urlpatterns.empty())
	{
		this->urlpatterns();
		for (auto& module : this->_sub_modules_to_init)
		{
			module();
		}
	}

	return this->_urlpatterns;
}

std::vector<std::shared_ptr<cmd::AbstractCommand>> ModuleConfig::get_commands()
{
	if (this->_commands.empty())
	{
		this->commands();
	}

	return this->_commands;
}

__CONF_END__
