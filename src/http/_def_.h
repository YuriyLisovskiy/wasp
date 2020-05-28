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
 * http/_def_.h
 *
 * Purpose: http module's definitions.
 */

#pragma once

#include "../_def_.h"


// http
#define __HTTP_BEGIN__ __MAIN_NAMESPACE_BEGIN__ namespace http {
#define __HTTP_END__ } __MAIN_NAMESPACE_END__

// http::internal
#define __HTTP_INTERNAL_BEGIN__ __HTTP_BEGIN__ namespace internal {
#define __HTTP_INTERNAL_END__ } __HTTP_END__
