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


/// wasp::core::utility
#define __UTILITY_BEGIN__ __CORE_BEGIN__ namespace utility {
#define __UTILITY_END__ } __CORE_END__


/// Declares exception's class with given base.
#define DEF_WASP_EXCEPTION_WITH_BASE(name, base, default_message)\
class name : public base\
{\
protected:\
	name(\
		const char* message, int line, const char* function, const char* file, const char* type\
	)\
		: base(message, line, function, file, type)\
	{\
	}\
\
public:\
	explicit name(\
		const char* message = default_message,\
		int line = 0, const char* function = "", const char* file = ""\
	)\
		: name(message, line, function, file, #name)\
	{\
	}\
\
	explicit name(\
		const std::string& message = default_message,\
		int line = 0, const char* function = "", const char* file = ""\
	)\
		: name(message.c_str(), line, function, file)\
	{\
	}\
}


#define DEF_WASP_HTTP_EXCEPTION(name, status_code, default_message)\
class name : public ErrorResponseException\
{\
protected:\
	name(\
		const char* message, int line, const char* function, const char* file, const char* type\
	)\
		: ErrorResponseException(status_code, message, line, function, file, type)\
	{\
	}\
\
public:\
	explicit name(\
		const char* message = default_message,\
		int line = 0, const char* function = "", const char* file = ""\
	)\
		: name(message, line, function, file, #name)\
	{\
	}\
\
	explicit name(\
		const std::string& message = default_message,\
		int line = 0, const char* function = "", const char* file = ""\
	)\
		: name(message.c_str(), line, function, file)\
	{\
	}\
}
