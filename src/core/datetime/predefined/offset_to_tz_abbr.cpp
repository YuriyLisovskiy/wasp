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
 * An implementation of core/datetime/predefined/offset_to_tz_abbr.h
 */

#include "./offset_to_tz_abbr.h"


__DATETIME_INTERNAL_BEGIN__

collections::Dict<std::string, std::string>
OFFSET_TO_TZ_ABBR(std::map<std::string, std::string>
{
	{"+0100", "A"},		    // Alpha Time Zone
	{"+1030", "ACDT"},		// Australian Central Daylight Time
	{"+0930", "ACST"},		// Australian Central Standard Time
	{"+0845", "ACWT"},		// Australian Central Western Time
	{"-0300", "ADT"},		// Atlantic Daylight Time (Caribbean, North America)
	{"-0500", "ACT"},		// Acre Time (Brazil)
	{"+1100", "AEDT"},		// Australian Eastern Daylight Time
	{"+1000", "AEST"},		// Australian Eastern Standard Time
	{"+0430", "AFT"},		// Afghanistan Time
	{"-0800", "AKDT"},		// Alaska Daylight Time
	{"-0900", "AKST"},		// Alaska Standard Time
	{"+0600", "ALMT"},		// Alma-Ata Time
	{"+0400", "AMT"},		// Armenia Time
	{"+0500", "AMST"},		// Armenia Summer Time
	{"+1200", "ANAT"},		// Anadyr Time (Russia)
	{"+0900", "AWDT"},		// Australian Western Daylight Time
	{"+0800", "AWST"},		// Australian Western Standard Time
	{"-0100", "AZOT"},		// Azores Time
	{"-0200", "BRST"},		// Brasilia Summer Time
	{"+0630", "CCT"},		// Cocos Islands Time
	{"+1345", "CHADT"},		// Chatham Island Daylight Time
	{"+1245", "CHAST"},		// Chatham Island Standard Time
	{"-1000", "CKT"},		// Cook Island Time
	{"-0600", "CST"},		// Central Standard Time (North America & Central America)
	{"+0700", "CXT"},		// Christmas Island Time
	{"+0200", "EET"},		    // Bravo Time Zone
	{"+0300", "EEDT"},		    // Eastern European Daylight Time
	{"+1300", "FJST"},		// Fiji Summer Time
	{"+0000", "GMT"},		// Greenwich Mean Time
	{"+0330", "IRST"},		// Iran Standard Time
	{"+0530", "IST"},		// Indian Standard Time
	{"+1400", "LINT"},		// Line Islands Time
	{"-0930", "MART"},		// Marquesas Time
	{"-0700", "MST"},		// Mountain Standard Time (North America)
	{"-0230", "NDT"},		// Newfoundland Daylight Time
	{"+0545", "NPT"},		// Nepal Time
	{"-0330", "NST"},		// Newfoundland Standard Time
	{"-1100", "NUT"},		// Niue Time
	{"+0830", "PST"},		// Pyongyang Time (North Korea)
	{"-1200", "Y"}		    // Yankee Time Zone
});

__DATETIME_INTERNAL_END__
