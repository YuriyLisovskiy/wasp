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
 * _def_.h
 * Purpose: utility module definitions.
 */

#pragma once

#include "../_def_.h"


/// wasp::utils
#define __UTILS_BEGIN__ __WASP_BEGIN__ namespace utils {
#define __UTILS_END__ } __WASP_END__


/// wasp::utils::cache
#define __CACHE_BEGIN__ __UTILS_BEGIN__ namespace cache {
#define __CACHE_END__ } __UTILS_END__

/// wasp::utils::cache::internal
#define __CACHE_INTERNAL_BEGIN__ __CACHE_BEGIN__ namespace internal {
#define __CACHE_INTERNAL_END__ } __CACHE_END__


/// wasp::utils_http
#define __UTILS_HTTP_BEGIN__ __WASP_BEGIN__ namespace utils_http {
#define __UTILS_HTTP_END__ } __WASP_END__

/// wasp::utils_http::internal
#define __UTILS_HTTP_INTERNAL_BEGIN__ __UTILS_HTTP_BEGIN__ namespace internal {
#define __UTILS_HTTP_INTERNAL_END__ } __UTILS_HTTP_END__
