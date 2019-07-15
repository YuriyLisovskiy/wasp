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

/*
 * Offsets of known time zones.
 */

#ifndef WASP_UTILS_TZ_H
#define WASP_UTILS_TZ_H

#include <map>

#include "../globals.h"
#include "query_dict.h"


__INTERNAL_BEGIN__

const QueryDict TZ_OFFSET(std::map<std::string, int> {
// Afghanistan
	{"Asia/Kabul", 14430},

// Aland Islands
	{"Europe/Mariehamn", 10800},

// Albania
	{"Europe/Tirane", 7200},

// Algeria
	{"Africa/Algiers", 3600},

// American Samoa
	{"Pacific/Pago_Pago", -39600},

// Andorra
	{"Europe/Andorra", 7200},

// Angola
	{"Africa/Luanda", 3600},

// Anguilla
	{"America/Anguilla", -14400},

// Antarctica
	{"Antarctica/Casey", 28800},

// Antarctica
	{"Antarctica/Davis", 25200},

// Antarctica
	{"Antarctica/DumontDUrville", 36000},

// Antarctica
	{"Antarctica/Mawson", 18000},

// Antarctica
	{"Antarctica/McMurdo", 43200},

// Antarctica
	{"Antarctica/Palmer", -10800},

// Antarctica
	{"Antarctica/Rothera", -10800},

// Antarctica
	{"Antarctica/Syowa", 10800},

// Antarctica
	{"Antarctica/Troll", 7200},

// Antarctica
	{"Antarctica/Vostok", 21600},

// Antigua and Barbuda
	{"America/Antigua", -14400},

// Argentina
	{"America/Argentina/Buenos_Aires", -10800},

// Argentina
	{"America/Argentina/Catamarca", -10800},

// Argentina
	{"America/Argentina/Cordoba", -10800},

// Argentina
	{"America/Argentina/Jujuy", -10800},

// Argentina
	{"America/Argentina/La_Rioja", -10800},

// Argentina
	{"America/Argentina/Mendoza", -10800},

// Argentina
	{"America/Argentina/Rio_Gallegos", -10800},

// Argentina
	{"America/Argentina/Salta", -10800},

// Argentina
	{"America/Argentina/San_Juan", -10800},

// Argentina
	{"America/Argentina/San_Luis", -10800},

// Argentina
	{"America/Argentina/Tucuman", -10800},

// Argentina
	{"America/Argentina/Ushuaia", -10800},

// Armenia
	{"Asia/Yerevan", 14400},

// Aruba
	{"America/Aruba", -14400},

// Australia
	{"Antarctica/Macquarie", 39600},

// Australia
	{"Australia/Adelaide", 32430},

// Australia
	{"Australia/Brisbane", 36000},

// Australia
	{"Australia/Broken_Hill", 32430},

// Australia
	{"Australia/Currie", 36000},

// Australia
	{"Australia/Darwin", 32430},

// Australia
	{"Australia/Eucla", 28845},

// Australia
	{"Australia/Hobart", 36000},

// Australia
	{"Australia/Lindeman", 36000},

// Australia
	{"Australia/Lord_Howe", 36030},

// Australia
	{"Australia/Melbourne", 36000},

// Australia
	{"Australia/Perth", 28800},

// Australia
	{"Australia/Sydney", 36000},

// Austria
	{"Europe/Vienna", 7200},

// Azerbaijan
	{"Asia/Baku", 14400},

// Bahamas
	{"America/Nassau", -14400},

// Bahrain
	{"Asia/Bahrain", 10800},

// Bangladesh
	{"Asia/Dhaka", 21600},

// Barbados
	{"America/Barbados", -14400},

// Belarus
	{"Europe/Minsk", 10800},

// Belgium
	{"Europe/Brussels", 7200},

// Belize
	{"America/Belize", -21600},

// Benin
	{"Africa/Porto-Novo", 3600},

// Bermuda
	{"Atlantic/Bermuda", -10800},

// Bhutan
	{"Asia/Thimphu", 21600},

// Bolivia
	{"America/La_Paz", -14400},

// Bonaire, Saint Eustatius and Saba
	{"America/Kralendijk", -14400},

// Bosnia and Herzegovina
	{"Europe/Sarajevo", 7200},

// Botswana
	{"Africa/Gaborone", 7200},

// Brazil
	{"America/Araguaina", -10800},

// Brazil
	{"America/Bahia", -10800},

// Brazil
	{"America/Belem", -10800},

// Brazil
	{"America/Boa_Vista", -14400},

// Brazil
	{"America/Campo_Grande", -14400},

// Brazil
	{"America/Cuiaba", -14400},

// Brazil
	{"America/Eirunepe", -18000},

// Brazil
	{"America/Fortaleza", -10800},

// Brazil
	{"America/Maceio", -10800},

// Brazil
	{"America/Manaus", -14400},

// Brazil
	{"America/Noronha", -7200},

// Brazil
	{"America/Porto_Velho", -14400},

// Brazil
	{"America/Recife", -10800},

// Brazil
	{"America/Rio_Branco", -18000},

// Brazil
	{"America/Santarem", -10800},

// Brazil
	{"America/Sao_Paulo", -10800},

// British Indian Ocean Territory
	{"Indian/Chagos", 21600},

// British Virgin Islands
	{"America/Tortola", -14400},

// Brunei
	{"Asia/Brunei", 28800},

// Bulgaria
	{"Europe/Sofia", 10800},

// Burundi
	{"Africa/Bujumbura", 7200},

// Cambodia
	{"Asia/Phnom_Penh", 25200},

// Cameroon
	{"Africa/Douala", 3600},

// Canada
	{"America/Atikokan", -18000},

// Canada
	{"America/Blanc-Sablon", -14400},

// Canada
	{"America/Cambridge_Bay", -21600},

// Canada
	{"America/Creston", -25200},

// Canada
	{"America/Dawson", -25200},

// Canada
	{"America/Dawson_Creek", -25200},

// Canada
	{"America/Edmonton", -21600},

// Canada
	{"America/Fort_Nelson", -25200},

// Canada
	{"America/Glace_Bay", -10800},

// Canada
	{"America/Goose_Bay", -10800},

// Canada
	{"America/Halifax", -10800},

// Canada
	{"America/Inuvik", -21600},

// Canada
	{"America/Iqaluit", -14400},

// Canada
	{"America/Moncton", -10800},

// Canada
	{"America/Nipigon", -14400},

// Canada
	{"America/Pangnirtung", -14400},

// Canada
	{"America/Rainy_River", -18000},

// Canada
	{"America/Rankin_Inlet", -18000},

// Canada
	{"America/Regina", -21600},

// Canada
	{"America/Resolute", -18000},

// Canada
	{"America/St_Johns", -7170},

// Canada
	{"America/Swift_Current", -21600},

// Canada
	{"America/Thunder_Bay", -14400},

// Canada
	{"America/Toronto", -14400},

// Canada
	{"America/Vancouver", -25200},

// Canada
	{"America/Whitehorse", -25200},

// Canada
	{"America/Winnipeg", -18000},

// Canada
	{"America/Yellowknife", -21600},

// Cape Verde
	{"Atlantic/Cape_Verde", -3600},

// Cayman Islands
	{"America/Cayman", -18000},

// Central African Republic
	{"Africa/Bangui", 3600},

// Chad
	{"Africa/Ndjamena", 3600},

// Chile
	{"America/Punta_Arenas", -10800},

// Chile
	{"America/Santiago", -14400},

// Chile
	{"Pacific/Easter", -21600},

// China
	{"Asia/Shanghai", 28800},

// China
	{"Asia/Urumqi", 21600},

// Christmas Island
	{"Indian/Christmas", 25200},

// Cocos Islands
	{"Indian/Cocos", 21630},

// Colombia
	{"America/Bogota", -18000},

// Comoros
	{"Indian/Comoro", 10800},

// Cook Islands
	{"Pacific/Rarotonga", -36000},

// Costa Rica
	{"America/Costa_Rica", -21600},

// Croatia
	{"Europe/Zagreb", 7200},

// Cuba
	{"America/Havana", -14400},

// Cura\xc3\xa7ao
	{"America/Curacao", -14400},

// Cyprus
	{"Asia/Famagusta", 10800},

// Cyprus
	{"Asia/Nicosia", 10800},

// Czech Republic
	{"Europe/Prague", 7200},

// Democratic Republic of the Congo
	{"Africa/Kinshasa", 3600},

// Democratic Republic of the Congo
	{"Africa/Lubumbashi", 7200},

// Denmark
	{"Europe/Copenhagen", 7200},

// Djibouti
	{"Africa/Djibouti", 10800},

// Dominica
	{"America/Dominica", -14400},

// Dominican Republic
	{"America/Santo_Domingo", -14400},

// East Timor
	{"Asia/Dili", 32400},

// Ecuador
	{"America/Guayaquil", -18000},

// Ecuador
	{"Pacific/Galapagos", -21600},

// Egypt
	{"Africa/Cairo", 7200},

// El Salvador
	{"America/El_Salvador", -21600},

// Equatorial Guinea
	{"Africa/Malabo", 3600},

// Eritrea
	{"Africa/Asmara", 10800},

// Estonia
	{"Europe/Tallinn", 10800},

// Ethiopia
	{"Africa/Addis_Ababa", 10800},

// Falkland Islands
	{"Atlantic/Stanley", -10800},

// Faroe Islands
	{"Atlantic/Faroe", 3600},

// Fiji
	{"Pacific/Fiji", 43200},

// Finland
	{"Europe/Helsinki", 10800},

// France
	{"Europe/Paris", 7200},

// French Guiana
	{"America/Cayenne", -10800},

// French Polynesia
	{"Pacific/Gambier", -32400},

// French Polynesia
	{"Pacific/Marquesas", -32370},

// French Polynesia
	{"Pacific/Tahiti", -36000},

// French Southern Territories
	{"Indian/Kerguelen", 18000},

// Gabon
	{"Africa/Libreville", 3600},

// Georgia
	{"Asia/Tbilisi", 14400},

// Germany
	{"Europe/Berlin", 7200},

// Germany
	{"Europe/Busingen", 7200},

// Gibraltar
	{"Europe/Gibraltar", 7200},

// Greece
	{"Europe/Athens", 10800},

// Greenland
	{"America/Godthab", -7200},

// Greenland
	{"America/Thule", -10800},

// Grenada
	{"America/Grenada", -14400},

// Guadeloupe
	{"America/Guadeloupe", -14400},

// Guam
	{"Pacific/Guam", 36000},

// Guatemala
	{"America/Guatemala", -21600},

// Guernsey
	{"Europe/Guernsey", 3600},

// Guyana
	{"America/Guyana", -14400},

// Haiti
	{"America/Port-au-Prince", -14400},

// Honduras
	{"America/Tegucigalpa", -21600},

// Hong Kong
	{"Asia/Hong_Kong", 28800},

// Hungary
	{"Europe/Budapest", 7200},

// India
	{"Asia/Kolkata", 18030},

// Indonesia
	{"Asia/Jakarta", 25200},

// Indonesia
	{"Asia/Jayapura", 32400},

// Indonesia
	{"Asia/Makassar", 28800},

// Indonesia
	{"Asia/Pontianak", 25200},

// Iran
	{"Asia/Tehran", 14430},

// Iraq
	{"Asia/Baghdad", 10800},

// Ireland
	{"Europe/Dublin", 3600},

// Isle of Man
	{"Europe/Isle_of_Man", 3600},

// Israel
	{"Asia/Jerusalem", 10800},

// Italy
	{"Europe/Rome", 7200},

// Jamaica
	{"America/Jamaica", -18000},

// Japan
	{"Asia/Tokyo", 32400},

// Jersey
	{"Europe/Jersey", 3600},

// Jordan
	{"Asia/Amman", 10800},

// Kazakhstan
	{"Asia/Almaty", 21600},

// Kazakhstan
	{"Asia/Aqtau", 18000},

// Kazakhstan
	{"Asia/Aqtobe", 18000},

// Kazakhstan
	{"Asia/Atyrau", 18000},

// Kazakhstan
	{"Asia/Oral", 18000},

// Kazakhstan
	{"Asia/Qostanay", 21600},

// Kazakhstan
	{"Asia/Qyzylorda", 18000},

// Kenya
	{"Africa/Nairobi", 10800},

// Kiribati
	{"Pacific/Enderbury", 46800},

// Kiribati
	{"Pacific/Kiritimati", 50400},

// Kiribati
	{"Pacific/Tarawa", 43200},

// Kuwait
	{"Asia/Kuwait", 10800},

// Kyrgyzstan
	{"Asia/Bishkek", 21600},

// Laos
	{"Asia/Vientiane", 25200},

// Latvia
	{"Europe/Riga", 10800},

// Lebanon
	{"Asia/Beirut", 10800},

// Lesotho
	{"Africa/Maseru", 7200},

// Libya
	{"Africa/Tripoli", 7200},

// Liechtenstein
	{"Europe/Vaduz", 7200},

// Lithuania
	{"Europe/Vilnius", 10800},

// Luxembourg
	{"Europe/Luxembourg", 7200},

// Macao
	{"Asia/Macau", 28800},

// Macedonia
	{"Europe/Skopje", 7200},

// Madagascar
	{"Indian/Antananarivo", 10800},

// Malawi
	{"Africa/Blantyre", 7200},

// Malaysia
	{"Asia/Kuala_Lumpur", 28800},

// Malaysia
	{"Asia/Kuching", 28800},

// Maldives
	{"Indian/Maldives", 18000},

// Malta
	{"Europe/Malta", 7200},

// Marshall Islands
	{"Pacific/Kwajalein", 43200},

// Marshall Islands
	{"Pacific/Majuro", 43200},

// Martinique
	{"America/Martinique", -14400},

// Mauritius
	{"Indian/Mauritius", 14400},

// Mayotte
	{"Indian/Mayotte", 10800},

// Mexico
	{"America/Bahia_Banderas", -18000},

// Mexico
	{"America/Cancun", -18000},

// Mexico
	{"America/Chihuahua", -21600},

// Mexico
	{"America/Hermosillo", -25200},

// Mexico
	{"America/Matamoros", -18000},

// Mexico
	{"America/Mazatlan", -21600},

// Mexico
	{"America/Merida", -18000},

// Mexico
	{"America/Mexico_City", -18000},

// Mexico
	{"America/Monterrey", -18000},

// Mexico
	{"America/Ojinaga", -21600},

// Mexico
	{"America/Tijuana", -25200},

// Micronesia
	{"Pacific/Chuuk", 36000},

// Micronesia
	{"Pacific/Kosrae", 39600},

// Micronesia
	{"Pacific/Pohnpei", 39600},

// Moldova
	{"Europe/Chisinau", 10800},

// Monaco
	{"Europe/Monaco", 7200},

// Mongolia
	{"Asia/Choibalsan", 28800},

// Mongolia
	{"Asia/Hovd", 25200},

// Mongolia
	{"Asia/Ulaanbaatar", 28800},

// Montenegro
	{"Europe/Podgorica", 7200},

// Montserrat
	{"America/Montserrat", -14400},

// Morocco
	{"Africa/Casablanca", 3600},

// Mozambique
	{"Africa/Maputo", 7200},

// Myanmar
	{"Asia/Yangon", 21630},

// Namibia
	{"Africa/Windhoek", 7200},

// Nauru
	{"Pacific/Nauru", 43200},

// Nepal
	{"Asia/Kathmandu", 18045},

// Netherlands
	{"Europe/Amsterdam", 7200},

// New Caledonia
	{"Pacific/Noumea", 39600},

// New Zealand
	{"Pacific/Auckland", 43200},

// New Zealand
	{"Pacific/Chatham", 43245},

// Nicaragua
	{"America/Managua", -21600},

// Niger
	{"Africa/Niamey", 3600},

// Nigeria
	{"Africa/Lagos", 3600},

// Niue
	{"Pacific/Niue", -39600},

// Norfolk Island
	{"Pacific/Norfolk", 39600},

// North Korea
	{"Asia/Pyongyang", 32400},

// Northern Mariana Islands
	{"Pacific/Saipan", 36000},

// Norway
	{"Europe/Oslo", 7200},

// Oman
	{"Asia/Muscat", 14400},

// Pakistan
	{"Asia/Karachi", 18000},

// Palau
	{"Pacific/Palau", 32400},

// Palestinian Territory
	{"Asia/Gaza", 10800},

// Palestinian Territory
	{"Asia/Hebron", 10800},

// Panama
	{"America/Panama", -18000},

// Papua New Guinea
	{"Pacific/Bougainville", 39600},

// Papua New Guinea
	{"Pacific/Port_Moresby", 36000},

// Paraguay
	{"America/Asuncion", -14400},

// Peru
	{"America/Lima", -18000},

// Philippines
	{"Asia/Manila", 28800},

// Pitcairn
	{"Pacific/Pitcairn", -28800},

// Poland
	{"Europe/Warsaw", 7200},

// Portugal
	{"Atlantic/Madeira", 3600},

// Portugal
	{"Europe/Lisbon", 3600},

// Puerto Rico
	{"America/Puerto_Rico", -14400},

// Qatar
	{"Asia/Qatar", 10800},

// Republic of the Congo
	{"Africa/Brazzaville", 3600},

// Reunion
	{"Indian/Reunion", 14400},

// Romania
	{"Europe/Bucharest", 10800},

// Russia
	{"Asia/Anadyr", 43200},

// Russia
	{"Asia/Barnaul", 25200},

// Russia
	{"Asia/Chita", 32400},

// Russia
	{"Asia/Irkutsk", 28800},

// Russia
	{"Asia/Kamchatka", 43200},

// Russia
	{"Asia/Khandyga", 32400},

// Russia
	{"Asia/Krasnoyarsk", 25200},

// Russia
	{"Asia/Magadan", 39600},

// Russia
	{"Asia/Novokuznetsk", 25200},

// Russia
	{"Asia/Novosibirsk", 25200},

// Russia
	{"Asia/Omsk", 21600},

// Russia
	{"Asia/Sakhalin", 39600},

// Russia
	{"Asia/Srednekolymsk", 39600},

// Russia
	{"Asia/Tomsk", 25200},

// Russia
	{"Asia/Ust-Nera", 36000},

// Russia
	{"Asia/Vladivostok", 36000},

// Russia
	{"Asia/Yakutsk", 32400},

// Russia
	{"Asia/Yekaterinburg", 18000},

// Russia
	{"Europe/Astrakhan", 14400},

// Russia
	{"Europe/Kaliningrad", 7200},

// Russia
	{"Europe/Kirov", 10800},

// Russia
	{"Europe/Moscow", 10800},

// Russia
	{"Europe/Samara", 14400},

// Russia
	{"Europe/Saratov", 14400},

// Russia
	{"Europe/Ulyanovsk", 14400},

// Russia
	{"Europe/Volgograd", 14400},

// Rwanda
	{"Africa/Kigali", 7200},

// Saint Barth\xc3\xa9lemy
	{"America/St_Barthelemy", -14400},

// Saint Kitts and Nevis
	{"America/St_Kitts", -14400},

// Saint Lucia
	{"America/St_Lucia", -14400},

// Saint Martin
	{"America/Marigot", -14400},

// Saint Pierre and Miquelon
	{"America/Miquelon", -7200},

// Saint Vincent and the Grenadines
	{"America/St_Vincent", -14400},

// Samoa
	{"Pacific/Apia", 46800},

// San Marino
	{"Europe/San_Marino", 7200},

// Saudi Arabia
	{"Asia/Riyadh", 10800},

// Serbia
	{"Europe/Belgrade", 7200},

// Seychelles
	{"Indian/Mahe", 14400},

// Singapore
	{"Asia/Singapore", 28800},

// Sint Maarten
	{"America/Lower_Princes", -14400},

// Slovakia
	{"Europe/Bratislava", 7200},

// Slovenia
	{"Europe/Ljubljana", 7200},

// Solomon Islands
	{"Pacific/Guadalcanal", 39600},

// Somalia
	{"Africa/Mogadishu", 10800},

// South Africa
	{"Africa/Johannesburg", 7200},

// South Georgia and the South Sandwich Islands
	{"Atlantic/South_Georgia", -7200},

// South Korea
	{"Asia/Seoul", 32400},

// South Sudan
	{"Africa/Juba", 10800},

// Spain
	{"Africa/Ceuta", 7200},

// Spain
	{"Atlantic/Canary", 3600},

// Spain
	{"Europe/Madrid", 7200},

// Sri Lanka
	{"Asia/Colombo", 18030},

// Sudan
	{"Africa/Khartoum", 7200},

// Suriname
	{"America/Paramaribo", -10800},

// Svalbard and Jan Mayen
	{"Arctic/Longyearbyen", 7200},

// Swaziland
	{"Africa/Mbabane", 7200},

// Sweden
	{"Europe/Stockholm", 7200},

// Switzerland
	{"Europe/Zurich", 7200},

// Syria
	{"Asia/Damascus", 10800},

// Taiwan
	{"Asia/Taipei", 28800},

// Tajikistan
	{"Asia/Dushanbe", 18000},

// Tanzania
	{"Africa/Dar_es_Salaam", 10800},

// Thailand
	{"Asia/Bangkok", 25200},

// Tokelau
	{"Pacific/Fakaofo", 46800},

// Tonga
	{"Pacific/Tongatapu", 46800},

// Trinidad and Tobago
	{"America/Port_of_Spain", -14400},

// Tunisia
	{"Africa/Tunis", 3600},

// Turkey
	{"Europe/Istanbul", 10800},

// Turkmenistan
	{"Asia/Ashgabat", 18000},

// Turks and Caicos Islands
	{"America/Grand_Turk", -14400},

// Tuvalu
	{"Pacific/Funafuti", 43200},

// U.S. Virgin Islands
	{"America/St_Thomas", -14400},

// Uganda
	{"Africa/Kampala", 10800},

// Ukraine
	{"Europe/Kiev", 10800},

// Ukraine
	{"Europe/Simferopol", 10800},

// Ukraine
	{"Europe/Uzhgorod", 10800},

// Ukraine
	{"Europe/Zaporozhye", 10800},

// United Arab Emirates
	{"Asia/Dubai", 14400},

// United Kingdom
	{"Europe/London", 3600},

// United States
	{"America/Adak", -32400},

// United States
	{"America/Anchorage", -28800},

// United States
	{"America/Boise", -21600},

// United States
	{"America/Chicago", -18000},

// United States
	{"America/Denver", -21600},

// United States
	{"America/Detroit", -14400},

// United States
	{"America/Indiana/Indianapolis", -14400},

// United States
	{"America/Indiana/Knox", -18000},

// United States
	{"America/Indiana/Marengo", -14400},

// United States
	{"America/Indiana/Petersburg", -14400},

// United States
	{"America/Indiana/Tell_City", -18000},

// United States
	{"America/Indiana/Vevay", -14400},

// United States
	{"America/Indiana/Vincennes", -14400},

// United States
	{"America/Indiana/Winamac", -14400},

// United States
	{"America/Juneau", -28800},

// United States
	{"America/Kentucky/Louisville", -14400},

// United States
	{"America/Kentucky/Monticello", -14400},

// United States
	{"America/Los_Angeles", -25200},

// United States
	{"America/Menominee", -18000},

// United States
	{"America/Metlakatla", -28800},

// United States
	{"America/New_York", -14400},

// United States
	{"America/Nome", -28800},

// United States
	{"America/North_Dakota/Beulah", -18000},

// United States
	{"America/North_Dakota/Center", -18000},

// United States
	{"America/North_Dakota/New_Salem", -18000},

// United States
	{"America/Phoenix", -25200},

// United States
	{"America/Sitka", -28800},

// United States
	{"America/Yakutat", -28800},

// United States
	{"Pacific/Honolulu", -36000},

// United States Minor Outlying Islands
	{"Pacific/Midway", -39600},

// United States Minor Outlying Islands
	{"Pacific/Wake", 43200},

// Uruguay
	{"America/Montevideo", -10800},

// Uzbekistan
	{"Asia/Samarkand", 18000},

// Uzbekistan
	{"Asia/Tashkent", 18000},

// Vanuatu
	{"Pacific/Efate", 39600},

// Vatican
	{"Europe/Vatican", 7200},

// Venezuela
	{"America/Caracas", -14400},

// Vietnam
	{"Asia/Ho_Chi_Minh", 25200},

// Wallis and Futuna
	{"Pacific/Wallis", 43200},

// Western Sahara
	{"Africa/El_Aaiun", 3600},

// Yemen
	{"Asia/Aden", 10800},

// Zambia
	{"Africa/Lusaka", 7200},

// Zimbabwe
	{"Africa/Harare", 7200},
});

__INTERNAL_END__


#endif // WASP_UTILS_TZ_H
