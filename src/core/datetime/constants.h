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
 * constants.h
 * Purpose: offsets of known time zones.
 */

#ifndef WASP_UTILS_DATETIME_CONSTANTS_H
#define WASP_UTILS_DATETIME_CONSTANTS_H

#include <map>

#include "../../globals.h"
#include "../../collections/dict.h"


__DATETIME_BEGIN__

/// The smallest year number allowed in a date or datetime object.
const int MIN_YEAR = 1;

/// The largest year number allowed in a date or datetime object.
const int MAX_YEAR = 9999;

__DATETIME_END__


__INTERNAL_BEGIN__

extern Dict<std::string, int> TZ_TO_OFFSET;

__INTERNAL_END__


#endif // WASP_UTILS_DATETIME_CONSTANTS_H
