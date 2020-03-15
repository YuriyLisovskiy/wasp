/*
 * Copyright (c) 2020 Yuriy Lisovskiy
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

#pragma once

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "../render/library/base.h"
#include "./library/base.h"


__RENDER_BEGIN__

class BaseEngine : public IEngine
{
public:
	~BaseEngine() override = default;
	virtual lib::Filters& get_filters() = 0;
	virtual lib::Tags& get_tags() = 0;
};

__RENDER_END__
