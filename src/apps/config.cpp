/*
 * Copyright (c) 2019 Yuriy Lisovskiy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * An implementation of config.h.
 */

#include "./config.h"


__APPS_BEGIN__

std::vector<urls::UrlPattern> AppConfig::get_urlpatterns()
{
	if (this->_urlpatterns.empty())
	{
		this->urlpatterns();
	}

	return this->_urlpatterns;
}

std::vector<core::BaseCommand*> AppConfig::get_commands()
{
	if (this->_commands.empty())
	{
		this->commands();
	}

	return this->_commands;
}

void AppConfig::urlpatterns()
{
}

void AppConfig::commands()
{
}

void AppConfig::include(const std::string& prefix, AppConfig* app)
{
	auto included_urlpatterns = app->get_urlpatterns();
	for (const auto& pattern : included_urlpatterns)
	{
		this->_urlpatterns.emplace_back(
			str::rtrim(str::starts_with(prefix, "/") ? prefix : "/" + prefix, '/'),
			pattern
		);
	}
}

__APPS_END__
