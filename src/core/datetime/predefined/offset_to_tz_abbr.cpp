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
 * An implementation of offset_to_tz_abbr.h.
 */

#include "./offset_to_tz_abbr.h"


__DATETIME_INTERNAL_BEGIN__

collections::Dict<int, std::string> OFFSET_TO_TZ_ABBR(std::map<int, std::string>
{
	{0, "GMT"},
	{2*3600, "EET"},
	{3*3600, "EEDT"},
	{7*3600, "G"},
	{4*3600, "GST"},
	{11*3600, "KOST"},
	{-(9*3600 + 30*60), "MART"},
	// TODO: add data to OFFSET_TO_TZ_ABBR
});

__DATETIME_INTERNAL_END__
