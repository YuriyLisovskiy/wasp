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
 * utility.h
 * Purpose: some http helpers.
 */

#ifndef WASP_HTTP_UTILITY_H
#define WASP_HTTP_UTILITY_H

// C++ libraries.
#include <string>

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "../core/datetime/datetime.h"


__HTTP_BEGIN__

/// Converts std::string datetime to utc epoch in seconds.
extern size_t parse_http_datetime(const std::string& http_datetime);

/// Converts utc epoch seconds to std::string datetime.
extern std::string format_http_datetime(size_t epoch_seconds);

__HTTP_END__


#endif // WASP_HTTP_UTILITY_H
