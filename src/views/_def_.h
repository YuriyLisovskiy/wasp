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
 * views/_def_.h
 *
 * Purpose: views module's definitions.
 */

#pragma once

#include "../_def_.h"


/// wasp::views
#define __VIEWS_BEGIN__ __WASP_BEGIN__ namespace views {
#define __VIEWS_END__ } __WASP_END__

/// wasp::views::internal
#define __VIEWS_INTERNAL_BEGIN__ __VIEWS_BEGIN__ namespace internal {
#define __VIEWS_INTERNAL_END__ } __VIEWS_END__
