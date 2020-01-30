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
 * An implementation of tz_abbr_to_offset.h.
 */

#include "./tz_abbr_to_offset.h"


__DATETIME_INTERNAL_BEGIN__

collections::Dict<std::string, int> TZ_ABBR_TO_OFFSET(std::map<std::string, int>
{
	{"GMT", 0},
	{"EET", 2*3600},
	{"EEDT", 3*3600},
	{"G", 7*3600},
	{"GST", 4*3600},
	{"KOST", 11*3600},
	{"MART", -(9*3600 + 30*60)},
	// TODO: add data to TZ_ABBR_TO_OFFSET
});

__DATETIME_INTERNAL_END__
