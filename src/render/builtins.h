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

/**
 * render/builtins.h
 *
 * Purpose:
 * TODO: write render/builtins.h docs
 */

#pragma once

// C++ libraries.
#include <map>

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "../collections/dict.h"
#include "../core/object/object.h"


__RENDER_BEGIN__

typedef collections::Dict<std::string, core::object::Object*> KwArgs;

typedef std::function<core::object::Object*(const KwArgs& kwargs)> Filter;

extern collections::Dict<std::string, Filter> DEFAULT_FILTERS;

__RENDER_END__
