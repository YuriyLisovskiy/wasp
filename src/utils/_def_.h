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
 * utils/_def_.h
 *
 * Purpose: utility module definitions.
 */

#pragma once

#include "../_def_.h"


// utils
#define __UTILS_BEGIN__ __MAIN_NAMESPACE_BEGIN__ namespace utils {
#define __UTILS_END__ } __MAIN_NAMESPACE_END__


// utils::cache
#define __CACHE_BEGIN__ __UTILS_BEGIN__ namespace cache {
#define __CACHE_END__ } __UTILS_END__

// utils::cache::internal
#define __CACHE_INTERNAL_BEGIN__ __CACHE_BEGIN__ namespace internal {
#define __CACHE_INTERNAL_END__ } __CACHE_END__


// utils::fn
#define __FUNCTIONAL_BEGIN__ __UTILS_BEGIN__ namespace fn {
#define __FUNCTIONAL_END__ } __UTILS_END__


// utils_http
#define __UTILS_HTTP_BEGIN__ __MAIN_NAMESPACE_BEGIN__ namespace utils_http {
#define __UTILS_HTTP_END__ } __MAIN_NAMESPACE_END__

// utils_http::internal
#define __UTILS_HTTP_INTERNAL_BEGIN__ __UTILS_HTTP_BEGIN__ namespace internal {
#define __UTILS_HTTP_INTERNAL_END__ } __UTILS_HTTP_END__
