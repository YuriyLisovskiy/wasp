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
 * interface.h
 * Purpose: apps interfaces.
 */

#pragma once

// C++ libraries.
#include <string>
#include <vector>

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "../urls/pattern.h"
#include "../core/management/base.h"


__APPS_BEGIN__

class IAppConfig
{
public:
	virtual ~IAppConfig() = default;

	virtual std::string get_name() = 0;
	virtual std::vector<urls::UrlPattern> get_urlpatterns() = 0;
	virtual std::vector<core::BaseCommand*> get_commands() = 0;
};

__APPS_END__
