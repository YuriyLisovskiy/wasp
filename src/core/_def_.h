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
 * Purpose: core module's definitions.
 */

#pragma once

#include "../_def_.h"


/// wasp::core
#define __CORE_BEGIN__ __WASP_BEGIN__ namespace core {
#define __CORE_END__ } __WASP_END__

/// wasp::core::internal
#define __CORE_INTERNAL_BEGIN__ __CORE_BEGIN__ namespace internal {
#define __CORE_INTERNAL_END__ } __CORE_END__


/// wasp::core::path
#define __PATH_BEGIN__ __CORE_BEGIN__ namespace path {
#define __PATH_END__ } __CORE_END__

/// wasp::core::path::internal
#define __PATH_INTERNAL_BEGIN__ __PATH_BEGIN__ namespace internal {
#define __PATH_INTERNAL_END__ } __PATH_END__


/// wasp::core::mime
#define __MIME_BEGIN__ __CORE_BEGIN__ namespace mime {
#define __MIME_END__ } __CORE_END__


/// wasp::core::encoding
#define __ENCODING_BEGIN__ __CORE_BEGIN__ namespace encoding {
#define __ENCODING_END__ } __CORE_END__

/// wasp::core::encoding::internal
#define __ENCODING_INTERNAL_BEGIN__ __CORE_BEGIN__ namespace internal {
#define __ENCODING_INTERNAL_END__ } __CORE_END__


/// wasp::core::rgx
#define __RGX_BEGIN__ __CORE_BEGIN__ namespace rgx {
#define __RGX_END__ } __CORE_END__
