/*
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
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
 * apps/interface.h
 *
 * Purpose: apps interfaces.
 */

#pragma once

// Module definitions.
#include "./_def_.h"

// Framework modules.
#include "../urls/pattern.h"
#include "../core/management/base.h"


__URLS_BEGIN__

class UrlPattern;

__URLS_END__


__APPS_BEGIN__

class IAppConfig
{
public:
	virtual ~IAppConfig() = default;

	virtual bool is_initialized() const = 0;
	virtual std::string get_name() = 0;
	virtual std::string get_app_path() = 0;
	virtual std::vector<std::shared_ptr<urls::UrlPattern>> get_urlpatterns() = 0;
	virtual std::vector<std::shared_ptr<core::BaseCommand>> get_commands() = 0;
};

__APPS_END__
