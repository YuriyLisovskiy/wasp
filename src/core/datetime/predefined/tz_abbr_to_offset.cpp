/*
 * Copyright (c) 2020 Yuriy Lisovskiy
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

collections::Dict<std::string, std::string>
TZ_ABBR_TO_OFFSET(std::map<std::string, std::string>
{
	{"A", "+0100"},		    // Alpha Time Zone
	{"ACDT", "+1030"},		// Australian Central Daylight Time
	{"ACST", "+0930"},		// Australian Central Standard Time
	{"ACWT", "+0845"},		// Australian Central Western Time
	{"ADT", "-0300"},		// Atlantic Daylight Time (Caribbean, North America)
	{"ACT", "-0500"},		// Acre Time (Brazil)
	{"AEDT", "+1100"},		// Australian Eastern Daylight Time
	{"AEST", "+1000"},		// Australian Eastern Standard Time
	{"AFT", "+0430"},		// Afghanistan Time
	{"AKDT", "-0800"},		// Alaska Daylight Time
	{"AKST", "-0900"},		// Alaska Standard Time
	{"ALMT", "+0600"},		// Alma-Ata Time
	{"AMT", "+0400"},		// Armenia Time
 	{"AMST", "+0500"},		// Armenia Summer Time
	{"ANAT", "+1200"},		// Anadyr Time (Russia)
	{"AWDT", "+0900"},		// Australian Western Daylight Time
	{"AWST", "+0800"},		// Australian Western Standard Time
	{"AZOT", "-0100"},		// Azores Time
	{"BRST", "-0200"},		// Brasilia Summer Time
	{"CCT", "+0630"},		// Cocos Islands Time
	{"CHADT", "+1345"},		// Chatham Island Daylight Time
	{"CHAST", "+1245"},		// Chatham Island Standard Time
	{"CKT", "-1000"},		// Cook Island Time
 	{"CST", "-0600"},		// Central Standard Time (North America & Central America)
	{"CXT", "+0700"},		// Christmas Island Time
	{"EET", "+0200"},		    // Bravo Time Zone
	{"EEDT", "+0300"},		    // Eastern European Daylight Time
	{"FJST", "+1300"},		// Fiji Summer Time
	{"GMT", "+0000"},		// Greenwich Mean Time
	{"IRST", "+0330"},		// Iran Standard Time
	{"IST", "+0530"},		// Indian Standard Time
	{"LINT", "+1400"},		// Line Islands Time
	{"MART", "-0930"},		// Marquesas Time
	{"MST", "-0700"},		// Mountain Standard Time (North America)
	{"NDT", "-0230"},		// Newfoundland Daylight Time
	{"NPT", "+0545"},		// Nepal Time
	{"NST", "-0330"},		// Newfoundland Standard Time
	{"NUT", "-1100"},		// Niue Time
	{"PST", "+0830"},		// Pyongyang Time (North Korea)
	{"Y", "-1200"}		    // Yankee Time Zone
});

__DATETIME_INTERNAL_END__
