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

#ifndef WASP_UTILS__DATETIME_CONSTANTS_H
#define WASP_UTILS__DATETIME_CONSTANTS_H

#include <map>

#include "../../globals.h"
#include "../query_dict.h"


__DATETIME_BEGIN__

// The smallest year number allowed in a date or datetime object.
const int MIN_YEAR = 1;

// The largest year number allowed in a date or datetime object.
const int MAX_YEAR = 9999;

__DATETIME_END__


__INTERNAL_BEGIN__

const QueryDict TZ_TO_OFFSET(std::map<std::string, int> {
		{"Etc/GMT+12", -43200},                       // GMT-12:00
		{"Etc/GMT+11", -39600},                       // GMT-11:00
		{"Pacific/Midway", -39600},                   // Samoa Standard Time
		{"Pacific/Niue", -39600},                     // Niue Time
		{"Pacific/Pago_Pago", -39600},                // Samoa Standard Time
		{"Pacific/Samoa", -39600},                    // Samoa Standard Time
		{"US/Samoa", -39600},                         // Samoa Standard Time
		{"America/Adak", -36000},                     // Hawaii-Aleutian Standard Time
		{"America/Atka", -36000},                     // Hawaii-Aleutian Standard Time
		{"Etc/GMT+10", -36000},                       // GMT-10:00
		{"HST", -36000},                              // Hawaii Standard Time
		{"Pacific/Fakaofo", -36000},                  // Tokelau Time
		{"Pacific/Honolulu", -36000},                 // Hawaii Standard Time
		{"Pacific/Johnston", -36000},                 // Hawaii Standard Time
		{"Pacific/Rarotonga", -36000},                // Cook Is. Time
		{"Pacific/Tahiti", -36000},                   // Tahiti Time
		{"SystemV/HST10", -36000},                    // Hawaii Standard Time
		{"US/Aleutian", -36000},                      // Hawaii-Aleutian Standard Time
		{"US/Hawaii", -36000},                        // Hawaii Standard Time
		{"Pacific/Marquesas", -34200},                // Marquesas Time
		{"AST", -32400},                              // Alaska Standard Time
		{"America/Anchorage", -32400},                // Alaska Standard Time
		{"America/Juneau", -32400},                   // Alaska Standard Time
		{"America/Nome", -32400},                     // Alaska Standard Time
		{"America/Sitka", -32400},                    // Alaska Standard Time
		{"America/Yakutat", -32400},                  // Alaska Standard Time
		{"Etc/GMT+9", -32400},                        // GMT-09:00
		{"Pacific/Gambier", -32400},                  // Gambier Time
		{"SystemV/YST9", -32400},                     // Alaska Standard Time
		{"SystemV/YST9YDT", -32400},                  // Alaska Standard Time
		{"US/Alaska", -32400},                        // Alaska Standard Time
		{"America/Dawson", -28800},                   // Pacific Standard Time
		{"America/Ensenada", -28800},                 // Pacific Standard Time
		{"America/Los_Angeles", -28800},              // Pacific Standard Time
		{"America/Metlakatla", -28800},               // Metlakatla Standard Time
		{"America/Santa_Isabel", -28800},             // Pacific Standard Time
		{"America/Tijuana", -28800},                  // Pacific Standard Time
		{"America/Vancouver", -28800},                // Pacific Standard Time
		{"America/Whitehorse", -28800},               // Pacific Standard Time
		{"Canada/Pacific", -28800},                   // Pacific Standard Time
		{"Canada/Yukon", -28800},                     // Pacific Standard Time
		{"Etc/GMT+8", -28800},                        // GMT-08:00
		{"Mexico/BajaNorte", -28800},                 // Pacific Standard Time
		{"PST", -28800},                              // Pacific Standard Time
		{"PST8PDT", -28800},                          // Pacific Standard Time
		{"Pacific/Pitcairn", -28800},                 // Pitcairn Standard Time
		{"SystemV/PST8", -28800},                     // Pacific Standard Time
		{"SystemV/PST8PDT", -28800},                  // Pacific Standard Time
		{"US/Pacific", -28800},                       // Pacific Standard Time
		{"US/Pacific-New", -28800},                   // Pacific Standard Time
		{"America/Boise", -25200},                    // Mountain Standard Time
		{"America/Cambridge_Bay", -25200},            // Mountain Standard Time
		{"America/Chihuahua", -25200},                // Mountain Standard Time
		{"America/Dawson_Creek", -25200},             // Mountain Standard Time
		{"America/Denver", -25200},                   // Mountain Standard Time
		{"America/Edmonton", -25200},                 // Mountain Standard Time
		{"America/Hermosillo", -25200},               // Mountain Standard Time
		{"America/Inuvik", -25200},                   // Mountain Standard Time
		{"America/Mazatlan", -25200},                 // Mountain Standard Time
		{"America/Ojinaga", -25200},                  // Mountain Standard Time
		{"America/Phoenix", -25200},                  // Mountain Standard Time
		{"America/Shiprock", -25200},                 // Mountain Standard Time
		{"America/Yellowknife", -25200},              // Mountain Standard Time
		{"Canada/Mountain", -25200},                  // Mountain Standard Time
		{"Etc/GMT+7", -25200},                        // GMT-07:00
		{"MST", -25200},                              // Mountain Standard Time
		{"MST7MDT", -25200},                          // Mountain Standard Time
		{"Mexico/BajaSur", -25200},                   // Mountain Standard Time
		{"Navajo", -25200},                           // Mountain Standard Time
		{"PNT", -25200},                              // Mountain Standard Time
		{"SystemV/MST7", -25200},                     // Mountain Standard Time
		{"SystemV/MST7MDT", -25200},                  // Mountain Standard Time
		{"US/Arizona", -25200},                       // Mountain Standard Time
		{"US/Mountain", -25200},                      // Mountain Standard Time
		{"America/Bahia_Banderas", -21600},           // Central Standard Time
		{"America/Belize", -21600},                   // Central Standard Time
		{"America/Cancun", -21600},                   // Central Standard Time
		{"America/Chicago", -21600},                  // Central Standard Time
		{"America/Costa_Rica", -21600},               // Central Standard Time
		{"America/El_Salvador", -21600},              // Central Standard Time
		{"America/Guatemala", -21600},                // Central Standard Time
		{"America/Indiana/Knox", -21600},             // Central Standard Time
		{"America/Indiana/Tell_City", -21600},        // Central Standard Time
		{"America/Knox_IN", -21600},                  // Central Standard Time
		{"America/Managua", -21600},                  // Central Standard Time
		{"America/Matamoros", -21600},                // Central Standard Time
		{"America/Menominee", -21600},                // Central Standard Time
		{"America/Merida", -21600},                   // Central Standard Time
		{"America/Mexico_City", -21600},              // Central Standard Time
		{"America/Monterrey", -21600},                // Central Standard Time
		{"America/North_Dakota/Beulah", -21600},      // Central Standard Time
		{"America/North_Dakota/Center", -21600},      // Central Standard Time
		{"America/North_Dakota/New_Salem", -21600},   // Central Standard Time
		{"America/Rainy_River", -21600},              // Central Standard Time
		{"America/Rankin_Inlet", -21600},             // Central Standard Time
		{"America/Regina", -21600},                   // Central Standard Time
		{"America/Resolute", -21600},                 // Eastern Standard Time
		{"America/Swift_Current", -21600},            // Central Standard Time
		{"America/Tegucigalpa", -21600},              // Central Standard Time
		{"America/Winnipeg", -21600},                 // Central Standard Time
		{"CST", -21600},                              // Central Standard Time
		{"CST6CDT", -21600},                          // Central Standard Time
		{"Canada/Central", -21600},                   // Central Standard Time
		{"Canada/East-Saskatchewan", -21600},         // Central Standard Time
		{"Canada/Saskatchewan", -21600},              // Central Standard Time
		{"Chile/EasterIsland", -21600},               // Easter Is. Time
		{"Etc/GMT+6", -21600},                        // GMT-06:00
		{"Mexico/General", -21600},                   // Central Standard Time
		{"Pacific/Easter", -21600},                   // Easter Is. Time
		{"Pacific/Galapagos", -21600},                // Galapagos Time
		{"SystemV/CST6", -21600},                     // Central Standard Time
		{"SystemV/CST6CDT", -21600},                  // Central Standard Time
		{"US/Central", -21600},                       // Central Standard Time
		{"US/Indiana-Starke", -21600},                // Central Standard Time
		{"America/Atikokan", -18000},                 // Eastern Standard Time
		{"America/Bogota", -18000},                   // Colombia Time
		{"America/Cayman", -18000},                   // Eastern Standard Time
		{"America/Coral_Harbour", -18000},            // Eastern Standard Time
		{"America/Detroit", -18000},                  // Eastern Standard Time
		{"America/Fort_Wayne", -18000},               // Eastern Standard Time
		{"America/Grand_Turk", -18000},               // Eastern Standard Time
		{"America/Guayaquil", -18000},                // Ecuador Time
		{"America/Havana", -18000},                   // Cuba Standard Time
		{"America/Indiana/Indianapolis", -18000},     // Eastern Standard Time
		{"America/Indiana/Marengo", -18000},          // Eastern Standard Time
		{"America/Indiana/Petersburg", -18000},       // Eastern Standard Time
		{"America/Indiana/Vevay", -18000},            // Eastern Standard Time
		{"America/Indiana/Vincennes", -18000},        // Eastern Standard Time
		{"America/Indiana/Winamac", -18000},          // Eastern Standard Time
		{"America/Indianapolis", -18000},             // Eastern Standard Time
		{"America/Iqaluit", -18000},                  // Eastern Standard Time
		{"America/Jamaica", -18000},                  // Eastern Standard Time
		{"America/Kentucky/Louisville", -18000},      // Eastern Standard Time
		{"America/Kentucky/Monticello", -18000},      // Eastern Standard Time
		{"America/Lima", -18000},                     // Peru Time
		{"America/Louisville", -18000},               // Eastern Standard Time
		{"America/Montreal", -18000},                 // Eastern Standard Time
		{"America/Nassau", -18000},                   // Eastern Standard Time
		{"America/New_York", -18000},                 // Eastern Standard Time
		{"America/Nipigon", -18000},                  // Eastern Standard Time
		{"America/Panama", -18000},                   // Eastern Standard Time
		{"America/Pangnirtung", -18000},              // Eastern Standard Time
		{"America/Port-au-Prince", -18000},           // Eastern Standard Time
		{"America/Thunder_Bay", -18000},              // Eastern Standard Time
		{"America/Toronto", -18000},                  // Eastern Standard Time
		{"Canada/Eastern", -18000},                   // Eastern Standard Time
		{"Cuba", -18000},                             // Cuba Standard Time
		{"EST", -18000},                              // Eastern Standard Time
		{"EST5EDT", -18000},                          // Eastern Standard Time
		{"Etc/GMT+5", -18000},                        // GMT-05:00
		{"IET", -18000},                              // Eastern Standard Time
		{"Jamaica", -18000},                          // Eastern Standard Time
		{"SystemV/EST5", -18000},                     // Eastern Standard Time
		{"SystemV/EST5EDT", -18000},                  // Eastern Standard Time
		{"US/East-Indiana", -18000},                  // Eastern Standard Time
		{"US/Eastern", -18000},                       // Eastern Standard Time
		{"US/Michigan", -18000},                      // Eastern Standard Time
		{"America/Caracas", -16200},                  // Venezuela Time
		{"America/Anguilla", -14400},                 // Atlantic Standard Time
		{"America/Antigua", -14400},                  // Atlantic Standard Time
		{"America/Argentina/San_Luis", -14400},       // Western Argentine Time
		{"America/Aruba", -14400},                    // Atlantic Standard Time
		{"America/Asuncion", -14400},                 // Paraguay Time
		{"America/Barbados", -14400},                 // Atlantic Standard Time
		{"America/Blanc-Sablon", -14400},             // Atlantic Standard Time
		{"America/Boa_Vista", -14400},                // Amazon Time
		{"America/Campo_Grande", -14400},             // Amazon Time
		{"America/Cuiaba", -14400},                   // Amazon Time
		{"America/Curacao", -14400},                  // Atlantic Standard Time
		{"America/Dominica", -14400},                 // Atlantic Standard Time
		{"America/Eirunepe", -14400},                 // Amazon Time
		{"America/Glace_Bay", -14400},                // Atlantic Standard Time
		{"America/Goose_Bay", -14400},                // Atlantic Standard Time
		{"America/Grenada", -14400},                  // Atlantic Standard Time
		{"America/Guadeloupe", -14400},               // Atlantic Standard Time
		{"America/Guyana", -14400},                   // Guyana Time
		{"America/Halifax", -14400},                  // Atlantic Standard Time
		{"America/Kralendijk", -14400},               // GMT-04:00
		{"America/La_Paz", -14400},                   // Bolivia Time
		{"America/Lower_Princes", -14400},            // GMT-04:00
		{"America/Manaus", -14400},                   // Amazon Time
		{"America/Marigot", -14400},                  // Atlantic Standard Time
		{"America/Martinique", -14400},               // Atlantic Standard Time
		{"America/Moncton", -14400},                  // Atlantic Standard Time
		{"America/Montserrat", -14400},               // Atlantic Standard Time
		{"America/Port_of_Spain", -14400},            // Atlantic Standard Time
		{"America/Porto_Acre", -14400},               // Amazon Time
		{"America/Porto_Velho", -14400},              // Amazon Time
		{"America/Puerto_Rico", -14400},              // Atlantic Standard Time
		{"America/Rio_Branco", -14400},               // Amazon Time
		{"America/Santiago", -14400},                 // Chile Time
		{"America/Santo_Domingo", -14400},            // Atlantic Standard Time
		{"America/St_Barthelemy", -14400},            // Atlantic Standard Time
		{"America/St_Kitts", -14400},                 // Atlantic Standard Time
		{"America/St_Lucia", -14400},                 // Atlantic Standard Time
		{"America/St_Thomas", -14400},                // Atlantic Standard Time
		{"America/St_Vincent", -14400},               // Atlantic Standard Time
		{"America/Thule", -14400},                    // Atlantic Standard Time
		{"America/Tortola", -14400},                  // Atlantic Standard Time
		{"America/Virgin", -14400},                   // Atlantic Standard Time
		{"Antarctica/Palmer", -14400},                // Chile Time
		{"Atlantic/Bermuda", -14400},                 // Atlantic Standard Time
		{"Atlantic/Stanley", -14400},                 // Falkland Is. Time
		{"Brazil/Acre", -14400},                      // Amazon Time
		{"Brazil/West", -14400},                      // Amazon Time
		{"Canada/Atlantic", -14400},                  // Atlantic Standard Time
		{"Chile/Continental", -14400},                // Chile Time
		{"Etc/GMT+4", -14400},                        // GMT-04:00
		{"PRT", -14400},                              // Atlantic Standard Time
		{"SystemV/AST4", -14400},                     // Atlantic Standard Time
		{"SystemV/AST4ADT", -14400},                  // Atlantic Standard Time
		{"America/St_Johns", -12600},                 // Newfoundland Standard Time
		{"CNT", -12600},                              // Newfoundland Standard Time
		{"Canada/Newfoundland", -12600},              // Newfoundland Standard Time
		{"AGT", -10800},                              // Argentine Time
		{"America/Araguaina", -10800},                // Brasilia Time
		{"America/Argentina/Buenos_Aires", -10800},   // Argentine Time
		{"America/Argentina/Catamarca", -10800},      // Argentine Time
		{"America/Argentina/ComodRivadavia", -10800}, // Argentine Time
		{"America/Argentina/Cordoba", -10800},        // Argentine Time
		{"America/Argentina/Jujuy", -10800},          // Argentine Time
		{"America/Argentina/La_Rioja", -10800},       // Argentine Time
		{"America/Argentina/Mendoza", -10800},        // Argentine Time
		{"America/Argentina/Rio_Gallegos", -10800},   // Argentine Time
		{"America/Argentina/Salta", -10800},          // Argentine Time
		{"America/Argentina/San_Juan", -10800},       // Argentine Time
		{"America/Argentina/Tucuman", -10800},        // Argentine Time
		{"America/Argentina/Ushuaia", -10800},        // Argentine Time
		{"America/Bahia", -10800},                    // Brasilia Time
		{"America/Belem", -10800},                    // Brasilia Time
		{"America/Buenos_Aires", -10800},             // Argentine Time
		{"America/Catamarca", -10800},                // Argentine Time
		{"America/Cayenne", -10800},                  // French Guiana Time
		{"America/Cordoba", -10800},                  // Argentine Time
		{"America/Fortaleza", -10800},                // Brasilia Time
		{"America/Godthab", -10800},                  // Western Greenland Time
		{"America/Jujuy", -10800},                    // Argentine Time
		{"America/Maceio", -10800},                   // Brasilia Time
		{"America/Mendoza", -10800},                  // Argentine Time
		{"America/Miquelon", -10800},                 // Pierre & Miquelon Standard Time
		{"America/Montevideo", -10800},               // Uruguay Time
		{"America/Paramaribo", -10800},               // Suriname Time
		{"America/Recife", -10800},                   // Brasilia Time
		{"America/Rosario", -10800},                  // Argentine Time
		{"America/Santarem", -10800},                 // Brasilia Time
		{"America/Sao_Paulo", -10800},                // Brasilia Time
		{"Antarctica/Rothera", -10800},               // Rothera Time
		{"BET", -10800},                              // Brasilia Time
		{"Brazil/East", -10800},                      // Brasilia Time
		{"Etc/GMT+3", -10800},                        // GMT-03:00
		{"America/Noronha", -7200},                   // Fernando de Noronha Time
		{"Atlantic/South_Georgia", -7200},            // South Georgia Standard Time
		{"Brazil/DeNoronha", -7200},                  // Fernando de Noronha Time
		{"Etc/GMT+2", -7200},                         // GMT-02:00
		{"America/Scoresbysund", -3600},              // Eastern Greenland Time
		{"Atlantic/Azores", -3600},                   // Azores Time
		{"Atlantic/Cape_Verde", -3600},               // Cape Verde Time
		{"Etc/GMT+1", -3600},                         // GMT-01:00
		{"Africa/Abidjan", 0},                        // Greenwich Mean Time
		{"Africa/Accra", 0},                          // Ghana Mean Time
		{"Africa/Bamako", 0},                         // Greenwich Mean Time
		{"Africa/Banjul", 0},                         // Greenwich Mean Time
		{"Africa/Bissau", 0},                         // Greenwich Mean Time
		{"Africa/Casablanca", 0},                     // Western European Time
		{"Africa/Conakry", 0},                        // Greenwich Mean Time
		{"Africa/Dakar", 0},                          // Greenwich Mean Time
		{"Africa/El_Aaiun", 0},                       // Western European Time
		{"Africa/Freetown", 0},                       // Greenwich Mean Time
		{"Africa/Lome", 0},                           // Greenwich Mean Time
		{"Africa/Monrovia", 0},                       // Greenwich Mean Time
		{"Africa/Nouakchott", 0},                     // Greenwich Mean Time
		{"Africa/Ouagadougou", 0},                    // Greenwich Mean Time
		{"Africa/Sao_Tome", 0},                       // Greenwich Mean Time
		{"Africa/Timbuktu", 0},                       // Greenwich Mean Time
		{"America/Danmarkshavn", 0},                  // Greenwich Mean Time
		{"Atlantic/Canary", 0},                       // Western European Time
		{"Atlantic/Faeroe", 0},                       // Western European Time
		{"Atlantic/Faroe", 0},                        // Western European Time
		{"Atlantic/Madeira", 0},                      // Western European Time
		{"Atlantic/Reykjavik", 0},                    // Greenwich Mean Time
		{"Atlantic/St_Helena", 0},                    // Greenwich Mean Time
		{"Eire", 0},                                  // Greenwich Mean Time
		{"Etc/GMT", 0},                               // GMT+00:00
		{"Etc/GMT+0", 0},                             // GMT+00:00
		{"Etc/GMT-0", 0},                             // GMT+00:00
		{"Etc/GMT0", 0},                              // GMT+00:00
		{"Etc/Greenwich", 0},                         // Greenwich Mean Time
		{"Etc/UCT", 0},                               // Coordinated Universal Time
		{"Etc/UTC", 0},                               // Coordinated Universal Time
		{"Etc/Universal", 0},                         // Coordinated Universal Time
		{"Etc/Zulu", 0},                              // Coordinated Universal Time
		{"Europe/Belfast", 0},                        // Greenwich Mean Time
		{"Europe/Dublin", 0},                         // Greenwich Mean Time
		{"Europe/Guernsey", 0},                       // Greenwich Mean Time
		{"Europe/Isle_of_Man", 0},                    // Greenwich Mean Time
		{"Europe/Jersey", 0},                         // Greenwich Mean Time
		{"Europe/Lisbon", 0},                         // Western European Time
		{"Europe/London", 0},                         // Greenwich Mean Time
		{"GB", 0},                                    // Greenwich Mean Time
		{"GB-Eire", 0},                               // Greenwich Mean Time
		{"GMT", 0},                                   // Greenwich Mean Time
		{"GMT0", 0},                                  // GMT+00:00
		{"Greenwich", 0},                             // Greenwich Mean Time
		{"Iceland", 0},                               // Greenwich Mean Time
		{"Portugal", 0},                              // Western European Time
		{"UCT", 0},                                   // Coordinated Universal Time
		{"UTC", 0},                                   // Coordinated Universal Time
		{"Universal", 0},                             // Coordinated Universal Time
		{"WET", 0},                                   // Western European Time
		{"Zulu", 0},                                  // Coordinated Universal Time
		{"Africa/Algiers", 3600},                     // Central European Time
		{"Africa/Bangui", 3600},                      // Western African Time
		{"Africa/Brazzaville", 3600},                 // Western African Time
		{"Africa/Ceuta", 3600},                       // Central European Time
		{"Africa/Douala", 3600},                      // Western African Time
		{"Africa/Kinshasa", 3600},                    // Western African Time
		{"Africa/Lagos", 3600},                       // Western African Time
		{"Africa/Libreville", 3600},                  // Western African Time
		{"Africa/Luanda", 3600},                      // Western African Time
		{"Africa/Malabo", 3600},                      // Western African Time
		{"Africa/Ndjamena", 3600},                    // Western African Time
		{"Africa/Niamey", 3600},                      // Western African Time
		{"Africa/Porto-Novo", 3600},                  // Western African Time
		{"Africa/Tunis", 3600},                       // Central European Time
		{"Africa/Windhoek", 3600},                    // Western African Time
		{"Arctic/Longyearbyen", 3600},                // Central European Time
		{"Atlantic/Jan_Mayen", 3600},                 // Central European Time
		{"CET", 3600},                                // Central European Time
		{"ECT", 3600},                                // Central European Time
		{"Etc/GMT-1", 3600},                          // GMT+01:00
		{"Europe/Amsterdam", 3600},                   // Central European Time
		{"Europe/Andorra", 3600},                     // Central European Time
		{"Europe/Belgrade", 3600},                    // Central European Time
		{"Europe/Berlin", 3600},                      // Central European Time
		{"Europe/Bratislava", 3600},                  // Central European Time
		{"Europe/Brussels", 3600},                    // Central European Time
		{"Europe/Budapest", 3600},                    // Central European Time
		{"Europe/Copenhagen", 3600},                  // Central European Time
		{"Europe/Gibraltar", 3600},                   // Central European Time
		{"Europe/Ljubljana", 3600},                   // Central European Time
		{"Europe/Luxembourg", 3600},                  // Central European Time
		{"Europe/Madrid", 3600},                      // Central European Time
		{"Europe/Malta", 3600},                       // Central European Time
		{"Europe/Monaco", 3600},                      // Central European Time
		{"Europe/Oslo", 3600},                        // Central European Time
		{"Europe/Paris", 3600},                       // Central European Time
		{"Europe/Podgorica", 3600},                   // Central European Time
		{"Europe/Prague", 3600},                      // Central European Time
		{"Europe/Rome", 3600},                        // Central European Time
		{"Europe/San_Marino", 3600},                  // Central European Time
		{"Europe/Sarajevo", 3600},                    // Central European Time
		{"Europe/Skopje", 3600},                      // Central European Time
		{"Europe/Stockholm", 3600},                   // Central European Time
		{"Europe/Tirane", 3600},                      // Central European Time
		{"Europe/Vaduz", 3600},                       // Central European Time
		{"Europe/Vatican", 3600},                     // Central European Time
		{"Europe/Vienna", 3600},                      // Central European Time
		{"Europe/Warsaw", 3600},                      // Central European Time
		{"Europe/Zagreb", 3600},                      // Central European Time
		{"Europe/Zurich", 3600},                      // Central European Time
		{"MET", 3600},                                // Middle Europe Time
		{"Poland", 3600},                             // Central European Time
		{"ART", 7200},                                // Eastern European Time
		{"Africa/Blantyre", 7200},                    // Central African Time
		{"Africa/Bujumbura", 7200},                   // Central African Time
		{"Africa/Cairo", 7200},                       // Eastern European Time
		{"Africa/Gaborone", 7200},                    // Central African Time
		{"Africa/Harare", 7200},                      // Central African Time
		{"Africa/Johannesburg", 7200},                // South Africa Standard Time
		{"Africa/Kigali", 7200},                      // Central African Time
		{"Africa/Lubumbashi", 7200},                  // Central African Time
		{"Africa/Lusaka", 7200},                      // Central African Time
		{"Africa/Maputo", 7200},                      // Central African Time
		{"Africa/Maseru", 7200},                      // South Africa Standard Time
		{"Africa/Mbabane", 7200},                     // South Africa Standard Time
		{"Africa/Tripoli", 7200},                     // Eastern European Time
		{"Asia/Amman", 7200},                         // Eastern European Time
		{"Asia/Beirut", 7200},                        // Eastern European Time
		{"Asia/Damascus", 7200},                      // Eastern European Time
		{"Asia/Gaza", 7200},                          // Eastern European Time
		{"Asia/Hebron", 7200},                        // GMT+02:00
		{"Asia/Istanbul", 7200},                      // Eastern European Time
		{"Asia/Jerusalem", 7200},                     // Israel Standard Time
		{"Asia/Nicosia", 7200},                       // Eastern European Time
		{"Asia/Tel_Aviv", 7200},                      // Israel Standard Time
		{"CAT", 7200},                                // Central African Time
		{"EET", 7200},                                // Eastern European Time
		{"Egypt", 7200},                              // Eastern European Time
		{"Etc/GMT-2", 7200},                          // GMT+02:00
		{"Europe/Athens", 7200},                      // Eastern European Time
		{"Europe/Bucharest", 7200},                   // Eastern European Time
		{"Europe/Chisinau", 7200},                    // Eastern European Time
		{"Europe/Helsinki", 7200},                    // Eastern European Time
		{"Europe/Istanbul", 7200},                    // Eastern European Time
		{"Europe/Kiev", 7200},                        // Eastern European Time
		{"Europe/Mariehamn", 7200},                   // Eastern European Time
		{"Europe/Nicosia", 7200},                     // Eastern European Time
		{"Europe/Riga", 7200},                        // Eastern European Time
		{"Europe/Simferopol", 7200},                  // Eastern European Time
		{"Europe/Sofia", 7200},                       // Eastern European Time
		{"Europe/Tallinn", 7200},                     // Eastern European Time
		{"Europe/Tiraspol", 7200},                    // Eastern European Time
		{"Europe/Uzhgorod", 7200},                    // Eastern European Time
		{"Europe/Vilnius", 7200},                     // Eastern European Time
		{"Europe/Zaporozhye", 7200},                  // Eastern European Time
		{"Israel", 7200},                             // Israel Standard Time
		{"Libya", 7200},                              // Eastern European Time
		{"Turkey", 7200},                             // Eastern European Time
		{"Africa/Addis_Ababa", 10800},                // Eastern African Time
		{"Africa/Asmara", 10800},                     // Eastern African Time
		{"Africa/Asmera", 10800},                     // Eastern African Time
		{"Africa/Dar_es_Salaam", 10800},              // Eastern African Time
		{"Africa/Djibouti", 10800},                   // Eastern African Time
		{"Africa/Juba", 10800},                       // GMT+03:00
		{"Africa/Kampala", 10800},                    // Eastern African Time
		{"Africa/Khartoum", 10800},                   // Eastern African Time
		{"Africa/Mogadishu", 10800},                  // Eastern African Time
		{"Africa/Nairobi", 10800},                    // Eastern African Time
		{"Antarctica/Syowa", 10800},                  // Syowa Time
		{"Asia/Aden", 10800},                         // Arabia Standard Time
		{"Asia/Baghdad", 10800},                      // Arabia Standard Time
		{"Asia/Bahrain", 10800},                      // Arabia Standard Time
		{"Asia/Kuwait", 10800},                       // Arabia Standard Time
		{"Asia/Qatar", 10800},                        // Arabia Standard Time
		{"Asia/Riyadh", 10800},                       // Arabia Standard Time
		{"EAT", 10800},                               // Eastern African Time
		{"Etc/GMT-3", 10800},                         // GMT+03:00
		{"Europe/Kaliningrad", 10800},                // Eastern European Time
		{"Europe/Minsk", 10800},                      // Eastern European Time
		{"Indian/Antananarivo", 10800},               // Eastern African Time
		{"Indian/Comoro", 10800},                     // Eastern African Time
		{"Indian/Mayotte", 10800},                    // Eastern African Time
		{"Asia/Riyadh87", 11224},                     // GMT+03:07
		{"Asia/Riyadh88", 11224},                     // GMT+03:07
		{"Asia/Riyadh89", 11224},                     // GMT+03:07
		{"Mideast/Riyadh87", 11224},                  // GMT+03:07
		{"Mideast/Riyadh88", 11224},                  // GMT+03:07
		{"Mideast/Riyadh89", 11224},                  // GMT+03:07
		{"Asia/Tehran", 12600},                       // Iran Standard Time
		{"Iran", 12600},                              // Iran Standard Time
		{"Asia/Baku", 14400},                         // Azerbaijan Time
		{"Asia/Dubai", 14400},                        // Gulf Standard Time
		{"Asia/Muscat", 14400},                       // Gulf Standard Time
		{"Asia/Tbilisi", 14400},                      // Georgia Time
		{"Asia/Yerevan", 14400},                      // Armenia Time
		{"Etc/GMT-4", 14400},                         // GMT+04:00
		{"Europe/Moscow", 14400},                     // Moscow Standard Time
		{"Europe/Samara", 14400},                     // Samara Time
		{"Europe/Volgograd", 14400},                  // Volgograd Time
		{"Indian/Mahe", 14400},                       // Seychelles Time
		{"Indian/Mauritius", 14400},                  // Mauritius Time
		{"Indian/Reunion", 14400},                    // Reunion Time
		{"NET", 14400},                               // Armenia Time
		{"W-SU", 14400},                              // Moscow Standard Time
		{"Asia/Kabul", 16200},                        // Afghanistan Time
		{"Antarctica/Mawson", 18000},                 // Mawson Time
		{"Asia/Aqtau", 18000},                        // Aqtau Time
		{"Asia/Aqtobe", 18000},                       // Aqtobe Time
		{"Asia/Ashgabat", 18000},                     // Turkmenistan Time
		{"Asia/Ashkhabad", 18000},                    // Turkmenistan Time
		{"Asia/Dushanbe", 18000},                     // Tajikistan Time
		{"Asia/Karachi", 18000},                      // Pakistan Time
		{"Asia/Oral", 18000},                         // Oral Time
		{"Asia/Samarkand", 18000},                    // Uzbekistan Time
		{"Asia/Tashkent", 18000},                     // Uzbekistan Time
		{"Etc/GMT-5", 18000},                         // GMT+05:00
		{"Indian/Kerguelen", 18000},                  // French Southern & Antarctic Lands Time
		{"Indian/Maldives", 18000},                   // Maldives Time
		{"PLT", 18000},                               // Pakistan Time
		{"Asia/Calcutta", 19800},                     // India Standard Time
		{"Asia/Colombo", 19800},                      // India Standard Time
		{"Asia/Kolkata", 19800},                      // India Standard Time
		{"IST", 19800},                               // India Standard Time
		{"Asia/Kathmandu", 20700},                    // Nepal Time
		{"Asia/Katmandu", 20700},                     // Nepal Time
		{"Antarctica/Vostok", 21600},                 // Vostok Time
		{"Asia/Almaty", 21600},                       // Alma-Ata Time
		{"Asia/Bishkek", 21600},                      // Kirgizstan Time
		{"Asia/Dacca", 21600},                        // Bangladesh Time
		{"Asia/Dhaka", 21600},                        // Bangladesh Time
		{"Asia/Qyzylorda", 21600},                    // Qyzylorda Time
		{"Asia/Thimbu", 21600},                       // Bhutan Time
		{"Asia/Thimphu", 21600},                      // Bhutan Time
		{"Asia/Yekaterinburg", 21600},                // Yekaterinburg Time
		{"BST", 21600},                               // Bangladesh Time
		{"Etc/GMT-6", 21600},                         // GMT+06:00
		{"Indian/Chagos", 21600},                     // Indian Ocean Territory Time
		{"Asia/Rangoon", 23400},                      // Myanmar Time
		{"Indian/Cocos", 23400},                      // Cocos Islands Time
		{"Antarctica/Davis", 25200},                  // Davis Time
		{"Asia/Bangkok", 25200},                      // Indochina Time
		{"Asia/Ho_Chi_Minh", 25200},                  // Indochina Time
		{"Asia/Hovd", 25200},                         // Hovd Time
		{"Asia/Jakarta", 25200},                      // West Indonesia Time
		{"Asia/Novokuznetsk", 25200},                 // Novosibirsk Time
		{"Asia/Novosibirsk", 25200},                  // Novosibirsk Time
		{"Asia/Omsk", 25200},                         // Omsk Time
		{"Asia/Phnom_Penh", 25200},                   // Indochina Time
		{"Asia/Pontianak", 25200},                    // West Indonesia Time
		{"Asia/Saigon", 25200},                       // Indochina Time
		{"Asia/Vientiane", 25200},                    // Indochina Time
		{"Etc/GMT-7", 25200},                         // GMT+07:00
		{"Indian/Christmas", 25200},                  // Christmas Island Time
		{"VST", 25200},                               // Indochina Time
		{"Antarctica/Casey", 28800},                  // Western Standard Time (Australia)
		{"Asia/Brunei", 28800},                       // Brunei Time
		{"Asia/Choibalsan", 28800},                   // Choibalsan Time
		{"Asia/Chongqing", 28800},                    // China Standard Time
		{"Asia/Chungking", 28800},                    // China Standard Time
		{"Asia/Harbin", 28800},                       // China Standard Time
		{"Asia/Hong_Kong", 28800},                    // Hong Kong Time
		{"Asia/Kashgar", 28800},                      // China Standard Time
		{"Asia/Krasnoyarsk", 28800},                  // Krasnoyarsk Time
		{"Asia/Kuala_Lumpur", 28800},                 // Malaysia Time
		{"Asia/Kuching", 28800},                      // Malaysia Time
		{"Asia/Macao", 28800},                        // China Standard Time
		{"Asia/Macau", 28800},                        // China Standard Time
		{"Asia/Makassar", 28800},                     // Central Indonesia Time
		{"Asia/Manila", 28800},                       // Philippines Time
		{"Asia/Shanghai", 28800},                     // China Standard Time
		{"Asia/Singapore", 28800},                    // Singapore Time
		{"Asia/Taipei", 28800},                       // China Standard Time
		{"Asia/Ujung_Pandang", 28800},                // Central Indonesia Time
		{"Asia/Ulaanbaatar", 28800},                  // Ulaanbaatar Time
		{"Asia/Ulan_Bator", 28800},                   // Ulaanbaatar Time
		{"Asia/Urumqi", 28800},                       // China Standard Time
		{"Australia/Perth", 28800},                   // Western Standard Time (Australia)
		{"Australia/West", 28800},                    // Western Standard Time (Australia)
		{"CTT", 28800},                               // China Standard Time
		{"Etc/GMT-8", 28800},                         // GMT+08:00
		{"Hongkong", 28800},                          // Hong Kong Time
		{"PRC", 28800},                               // China Standard Time
		{"Singapore", 28800},                         // Singapore Time
		{"Australia/Eucla", 31500},                   // Central Western Standard Time (Australia)
		{"Asia/Dili", 32400},                         // Timor-Leste Time
		{"Asia/Irkutsk", 32400},                      // Irkutsk Time
		{"Asia/Jayapura", 32400},                     // East Indonesia Time
		{"Asia/Pyongyang", 32400},                    // Korea Standard Time
		{"Asia/Seoul", 32400},                        // Korea Standard Time
		{"Asia/Tokyo", 32400},                        // Japan Standard Time
		{"Etc/GMT-9", 32400},                         // GMT+09:00
		{"JST", 32400},                               // Japan Standard Time
		{"Japan", 32400},                             // Japan Standard Time
		{"Pacific/Palau", 32400},                     // Palau Time
		{"ROK", 32400},                               // Korea Standard Time
		{"ACT", 34200},                               // Central Standard Time (Northern Territory)
		{"Australia/Adelaide", 34200},                // Central Standard Time (South Australia)
		{"Australia/Broken_Hill", 34200},             // Central Standard Time (South Australia/New South Wales)
		{"Australia/Darwin", 34200},                  // Central Standard Time (Northern Territory)
		{"Australia/North", 34200},                   // Central Standard Time (Northern Territory)
		{"Australia/South", 34200},                   // Central Standard Time (South Australia)
		{"Australia/Yancowinna", 34200},              // Central Standard Time (South Australia/New South Wales)
		{"AET", 36000},                               // Eastern Standard Time (New South Wales)
		{"Antarctica/DumontDUrville", 36000},         // Dumont-d\'Urville Time
		{"Asia/Yakutsk", 36000},                      // Yakutsk Time
		{"Australia/ACT", 36000},                     // Eastern Standard Time (New South Wales)
		{"Australia/Brisbane", 36000},                // Eastern Standard Time (Queensland)
		{"Australia/Canberra", 36000},                // Eastern Standard Time (New South Wales)
		{"Australia/Currie", 36000},                  // Eastern Standard Time (New South Wales)
		{"Australia/Hobart", 36000},                  // Eastern Standard Time (Tasmania)
		{"Australia/Lindeman", 36000},                // Eastern Standard Time (Queensland)
		{"Australia/Melbourne", 36000},               // Eastern Standard Time (Victoria)
		{"Australia/NSW", 36000},                     // Eastern Standard Time (New South Wales)
		{"Australia/Queensland", 36000},              // Eastern Standard Time (Queensland)
		{"Australia/Sydney", 36000},                  // Eastern Standard Time (New South Wales)
		{"Australia/Tasmania", 36000},                // Eastern Standard Time (Tasmania)
		{"Australia/Victoria", 36000},                // Eastern Standard Time (Victoria)
		{"Etc/GMT-10", 36000},                        // GMT+10:00
		{"Pacific/Chuuk", 36000},                     // Chuuk Time
		{"Pacific/Guam", 36000},                      // Chamorro Standard Time
		{"Pacific/Port_Moresby", 36000},              // Papua New Guinea Time
		{"Pacific/Saipan", 36000},                    // Chamorro Standard Time
		{"Pacific/Truk", 36000},                      // Chuuk Time
		{"Pacific/Yap", 36000},                       // Chuuk Time
		{"Australia/LHI", 37800},                     // Lord Howe Standard Time
		{"Australia/Lord_Howe", 37800},               // Lord Howe Standard Time
		{"Antarctica/Macquarie", 39600},              // Macquarie Island Time
		{"Asia/Sakhalin", 39600},                     // Sakhalin Time
		{"Asia/Vladivostok", 39600},                  // Vladivostok Time
		{"Etc/GMT-11", 39600},                        // GMT+11:00
		{"Pacific/Efate", 39600},                     // Vanuatu Time
		{"Pacific/Guadalcanal", 39600},               // Solomon Is. Time
		{"Pacific/Kosrae", 39600},                    // Kosrae Time
		{"Pacific/Noumea", 39600},                    // New Caledonia Time
		{"Pacific/Pohnpei", 39600},                   // Pohnpei Time
		{"Pacific/Ponape", 39600},                    // Pohnpei Time
		{"SST", 39600},                               // Solomon Is. Time
		{"Pacific/Norfolk", 41400},                   // Norfolk Time
		{"Antarctica/McMurdo", 43200},                // New Zealand Standard Time
		{"Antarctica/South_Pole", 43200},             // New Zealand Standard Time
		{"Asia/Anadyr", 43200},                       // Anadyr Time
		{"Asia/Kamchatka", 43200},                    // Petropavlovsk-Kamchatski Time
		{"Asia/Magadan", 43200},                      // Magadan Time
		{"Etc/GMT-12", 43200},                        // GMT+12:00
		{"Kwajalein", 43200},                         // Marshall Islands Time
		{"NST", 43200},                               // New Zealand Standard Time
		{"NZ", 43200},                                // New Zealand Standard Time
		{"Pacific/Auckland", 43200},                  // New Zealand Standard Time
		{"Pacific/Fiji", 43200},                      // Fiji Time
		{"Pacific/Funafuti", 43200},                  // Tuvalu Time
		{"Pacific/Kwajalein", 43200},                 // Marshall Islands Time
		{"Pacific/Majuro", 43200},                    // Marshall Islands Time
		{"Pacific/Nauru", 43200},                     // Nauru Time
		{"Pacific/Tarawa", 43200},                    // Gilbert Is. Time
		{"Pacific/Wake", 43200},                      // Wake Time
		{"Pacific/Wallis", 43200},                    // Wallis & Futuna Time
		{"NZ-CHAT", 45900},                           // Chatham Standard Time
		{"Pacific/Chatham", 45900},                   // Chatham Standard Time
		{"Etc/GMT-13", 46800},                        // GMT+13:00
		{"MIT", 46800},                               // West Samoa Time
		{"Pacific/Apia", 46800},                      // West Samoa Time
		{"Pacific/Enderbury", 46800},                 // Phoenix Is. Time
		{"Pacific/Tongatapu", 46800},                 // Tonga Time
		{"Etc/GMT-14", 50400},                        // GMT+14:00
		{"Pacific/Kiritimati", 50400}                 // Line Is. Time
});

const QueryDict OFFSET_TO_TZ(std::map<int, std::string> {
		{-43200, "Etc/GMT+12"},                       // GMT-12:00
		{-39600, "Etc/GMT+11"},                       // GMT-11:00
		{-39600, "Pacific/Midway"},                   // Samoa Standard Time
		{-39600, "Pacific/Niue"},                     // Niue Time
		{-39600, "Pacific/Pago_Pago"},                // Samoa Standard Time
		{-39600, "Pacific/Samoa"},                    // Samoa Standard Time
		{-39600, "US/Samoa"},                         // Samoa Standard Time
		{-36000, "America/Adak"},                     // Hawaii-Aleutian Standard Time
		{-36000, "America/Atka"},                     // Hawaii-Aleutian Standard Time
		{-36000, "Etc/GMT+10"},                       // GMT-10:00
		{-36000, "HST"},                              // Hawaii Standard Time
		{-36000, "Pacific/Fakaofo"},                  // Tokelau Time
		{-36000, "Pacific/Honolulu"},                 // Hawaii Standard Time
		{-36000, "Pacific/Johnston"},                 // Hawaii Standard Time
		{-36000, "Pacific/Rarotonga"},                // Cook Is. Time
		{-36000, "Pacific/Tahiti"},                   // Tahiti Time
		{-36000, "SystemV/HST10"},                    // Hawaii Standard Time
		{-36000, "US/Aleutian"},                      // Hawaii-Aleutian Standard Time
		{-36000, "US/Hawaii"},                        // Hawaii Standard Time
		{-34200, "Pacific/Marquesas"},                // Marquesas Time
		{-32400, "AST"},                              // Alaska Standard Time
		{-32400, "America/Anchorage"},                // Alaska Standard Time
		{-32400, "America/Juneau"},                   // Alaska Standard Time
		{-32400, "America/Nome"},                     // Alaska Standard Time
		{-32400, "America/Sitka"},                    // Alaska Standard Time
		{-32400, "America/Yakutat"},                  // Alaska Standard Time
		{-32400, "Etc/GMT+9"},                        // GMT-09:00
		{-32400, "Pacific/Gambier"},                  // Gambier Time
		{-32400, "SystemV/YST9"},                     // Alaska Standard Time
		{-32400, "SystemV/YST9YDT"},                  // Alaska Standard Time
		{-32400, "US/Alaska"},                        // Alaska Standard Time
		{-28800, "America/Dawson"},                   // Pacific Standard Time
		{-28800, "America/Ensenada"},                 // Pacific Standard Time
		{-28800, "America/Los_Angeles"},              // Pacific Standard Time
		{-28800, "America/Metlakatla"},               // Metlakatla Standard Time
		{-28800, "America/Santa_Isabel"},             // Pacific Standard Time
		{-28800, "America/Tijuana"},                  // Pacific Standard Time
		{-28800, "America/Vancouver"},                // Pacific Standard Time
		{-28800, "America/Whitehorse"},               // Pacific Standard Time
		{-28800, "Canada/Pacific"},                   // Pacific Standard Time
		{-28800, "Canada/Yukon"},                     // Pacific Standard Time
		{-28800, "Etc/GMT+8"},                        // GMT-08:00
		{-28800, "Mexico/BajaNorte"},                 // Pacific Standard Time
		{-28800, "PST"},                              // Pacific Standard Time
		{-28800, "PST8PDT"},                          // Pacific Standard Time
		{-28800, "Pacific/Pitcairn"},                 // Pitcairn Standard Time
		{-28800, "SystemV/PST8"},                     // Pacific Standard Time
		{-28800, "SystemV/PST8PDT"},                  // Pacific Standard Time
		{-28800, "US/Pacific"},                       // Pacific Standard Time
		{-28800, "US/Pacific-New"},                   // Pacific Standard Time
		{-25200, "America/Boise"},                    // Mountain Standard Time
		{-25200, "America/Cambridge_Bay"},            // Mountain Standard Time
		{-25200, "America/Chihuahua"},                // Mountain Standard Time
		{-25200, "America/Dawson_Creek"},             // Mountain Standard Time
		{-25200, "America/Denver"},                   // Mountain Standard Time
		{-25200, "America/Edmonton"},                 // Mountain Standard Time
		{-25200, "America/Hermosillo"},               // Mountain Standard Time
		{-25200, "America/Inuvik"},                   // Mountain Standard Time
		{-25200, "America/Mazatlan"},                 // Mountain Standard Time
		{-25200, "America/Ojinaga"},                  // Mountain Standard Time
		{-25200, "America/Phoenix"},                  // Mountain Standard Time
		{-25200, "America/Shiprock"},                 // Mountain Standard Time
		{-25200, "America/Yellowknife"},              // Mountain Standard Time
		{-25200, "Canada/Mountain"},                  // Mountain Standard Time
		{-25200, "Etc/GMT+7"},                        // GMT-07:00
		{-25200, "MST"},                              // Mountain Standard Time
		{-25200, "MST7MDT"},                          // Mountain Standard Time
		{-25200, "Mexico/BajaSur"},                   // Mountain Standard Time
		{-25200, "Navajo"},                           // Mountain Standard Time
		{-25200, "PNT"},                              // Mountain Standard Time
		{-25200, "SystemV/MST7"},                     // Mountain Standard Time
		{-25200, "SystemV/MST7MDT"},                  // Mountain Standard Time
		{-25200, "US/Arizona"},                       // Mountain Standard Time
		{-25200, "US/Mountain"},                      // Mountain Standard Time
		{-21600, "America/Bahia_Banderas"},           // Central Standard Time
		{-21600, "America/Belize"},                   // Central Standard Time
		{-21600, "America/Cancun"},                   // Central Standard Time
		{-21600, "America/Chicago"},                  // Central Standard Time
		{-21600, "America/Costa_Rica"},               // Central Standard Time
		{-21600, "America/El_Salvador"},              // Central Standard Time
		{-21600, "America/Guatemala"},                // Central Standard Time
		{-21600, "America/Indiana/Knox"},             // Central Standard Time
		{-21600, "America/Indiana/Tell_City"},        // Central Standard Time
		{-21600, "America/Knox_IN"},                  // Central Standard Time
		{-21600, "America/Managua"},                  // Central Standard Time
		{-21600, "America/Matamoros"},                // Central Standard Time
		{-21600, "America/Menominee"},                // Central Standard Time
		{-21600, "America/Merida"},                   // Central Standard Time
		{-21600, "America/Mexico_City"},              // Central Standard Time
		{-21600, "America/Monterrey"},                // Central Standard Time
		{-21600, "America/North_Dakota/Beulah"},      // Central Standard Time
		{-21600, "America/North_Dakota/Center"},      // Central Standard Time
		{-21600, "America/North_Dakota/New_Salem"},   // Central Standard Time
		{-21600, "America/Rainy_River"},              // Central Standard Time
		{-21600, "America/Rankin_Inlet"},             // Central Standard Time
		{-21600, "America/Regina"},                   // Central Standard Time
		{-21600, "America/Resolute"},                 // Eastern Standard Time
		{-21600, "America/Swift_Current"},            // Central Standard Time
		{-21600, "America/Tegucigalpa"},              // Central Standard Time
		{-21600, "America/Winnipeg"},                 // Central Standard Time
		{-21600, "CST"},                              // Central Standard Time
		{-21600, "CST6CDT"},                          // Central Standard Time
		{-21600, "Canada/Central"},                   // Central Standard Time
		{-21600, "Canada/East-Saskatchewan"},         // Central Standard Time
		{-21600, "Canada/Saskatchewan"},              // Central Standard Time
		{-21600, "Chile/EasterIsland"},               // Easter Is. Time
		{-21600, "Etc/GMT+6"},                        // GMT-06:00
		{-21600, "Mexico/General"},                   // Central Standard Time
		{-21600, "Pacific/Easter"},                   // Easter Is. Time
		{-21600, "Pacific/Galapagos"},                // Galapagos Time
		{-21600, "SystemV/CST6"},                     // Central Standard Time
		{-21600, "SystemV/CST6CDT"},                  // Central Standard Time
		{-21600, "US/Central"},                       // Central Standard Time
		{-21600, "US/Indiana-Starke"},                // Central Standard Time
		{-18000, "America/Atikokan"},                 // Eastern Standard Time
		{-18000, "America/Bogota"},                   // Colombia Time
		{-18000, "America/Cayman"},                   // Eastern Standard Time
		{-18000, "America/Coral_Harbour"},            // Eastern Standard Time
		{-18000, "America/Detroit"},                  // Eastern Standard Time
		{-18000, "America/Fort_Wayne"},               // Eastern Standard Time
		{-18000, "America/Grand_Turk"},               // Eastern Standard Time
		{-18000, "America/Guayaquil"},                // Ecuador Time
		{-18000, "America/Havana"},                   // Cuba Standard Time
		{-18000, "America/Indiana/Indianapolis"},     // Eastern Standard Time
		{-18000, "America/Indiana/Marengo"},          // Eastern Standard Time
		{-18000, "America/Indiana/Petersburg"},       // Eastern Standard Time
		{-18000, "America/Indiana/Vevay"},            // Eastern Standard Time
		{-18000, "America/Indiana/Vincennes"},        // Eastern Standard Time
		{-18000, "America/Indiana/Winamac"},          // Eastern Standard Time
		{-18000, "America/Indianapolis"},             // Eastern Standard Time
		{-18000, "America/Iqaluit"},                  // Eastern Standard Time
		{-18000, "America/Jamaica"},                  // Eastern Standard Time
		{-18000, "America/Kentucky/Louisville"},      // Eastern Standard Time
		{-18000, "America/Kentucky/Monticello"},      // Eastern Standard Time
		{-18000, "America/Lima"},                     // Peru Time
		{-18000, "America/Louisville"},               // Eastern Standard Time
		{-18000, "America/Montreal"},                 // Eastern Standard Time
		{-18000, "America/Nassau"},                   // Eastern Standard Time
		{-18000, "America/New_York"},                 // Eastern Standard Time
		{-18000, "America/Nipigon"},                  // Eastern Standard Time
		{-18000, "America/Panama"},                   // Eastern Standard Time
		{-18000, "America/Pangnirtung"},              // Eastern Standard Time
		{-18000, "America/Port-au-Prince"},           // Eastern Standard Time
		{-18000, "America/Thunder_Bay"},              // Eastern Standard Time
		{-18000, "America/Toronto"},                  // Eastern Standard Time
		{-18000, "Canada/Eastern"},                   // Eastern Standard Time
		{-18000, "Cuba"},                             // Cuba Standard Time
		{-18000, "EST"},                              // Eastern Standard Time
		{-18000, "EST5EDT"},                          // Eastern Standard Time
		{-18000, "Etc/GMT+5"},                        // GMT-05:00
		{-18000, "IET"},                              // Eastern Standard Time
		{-18000, "Jamaica"},                          // Eastern Standard Time
		{-18000, "SystemV/EST5"},                     // Eastern Standard Time
		{-18000, "SystemV/EST5EDT"},                  // Eastern Standard Time
		{-18000, "US/East-Indiana"},                  // Eastern Standard Time
		{-18000, "US/Eastern"},                       // Eastern Standard Time
		{-18000, "US/Michigan"},                      // Eastern Standard Time
		{-16200, "America/Caracas"},                  // Venezuela Time
		{-14400, "America/Anguilla"},                 // Atlantic Standard Time
		{-14400, "America/Antigua"},                  // Atlantic Standard Time
		{-14400, "America/Argentina/San_Luis"},       // Western Argentine Time
		{-14400, "America/Aruba"},                    // Atlantic Standard Time
		{-14400, "America/Asuncion"},                 // Paraguay Time
		{-14400, "America/Barbados"},                 // Atlantic Standard Time
		{-14400, "America/Blanc-Sablon"},             // Atlantic Standard Time
		{-14400, "America/Boa_Vista"},                // Amazon Time
		{-14400, "America/Campo_Grande"},             // Amazon Time
		{-14400, "America/Cuiaba"},                   // Amazon Time
		{-14400, "America/Curacao"},                  // Atlantic Standard Time
		{-14400, "America/Dominica"},                 // Atlantic Standard Time
		{-14400, "America/Eirunepe"},                 // Amazon Time
		{-14400, "America/Glace_Bay"},                // Atlantic Standard Time
		{-14400, "America/Goose_Bay"},                // Atlantic Standard Time
		{-14400, "America/Grenada"},                  // Atlantic Standard Time
		{-14400, "America/Guadeloupe"},               // Atlantic Standard Time
		{-14400, "America/Guyana"},                   // Guyana Time
		{-14400, "America/Halifax"},                  // Atlantic Standard Time
		{-14400, "America/Kralendijk"},               // GMT-04:00
		{-14400, "America/La_Paz"},                   // Bolivia Time
		{-14400, "America/Lower_Princes"},            // GMT-04:00
		{-14400, "America/Manaus"},                   // Amazon Time
		{-14400, "America/Marigot"},                  // Atlantic Standard Time
		{-14400, "America/Martinique"},               // Atlantic Standard Time
		{-14400, "America/Moncton"},                  // Atlantic Standard Time
		{-14400, "America/Montserrat"},               // Atlantic Standard Time
		{-14400, "America/Port_of_Spain"},            // Atlantic Standard Time
		{-14400, "America/Porto_Acre"},               // Amazon Time
		{-14400, "America/Porto_Velho"},              // Amazon Time
		{-14400, "America/Puerto_Rico"},              // Atlantic Standard Time
		{-14400, "America/Rio_Branco"},               // Amazon Time
		{-14400, "America/Santiago"},                 // Chile Time
		{-14400, "America/Santo_Domingo"},            // Atlantic Standard Time
		{-14400, "America/St_Barthelemy"},            // Atlantic Standard Time
		{-14400, "America/St_Kitts"},                 // Atlantic Standard Time
		{-14400, "America/St_Lucia"},                 // Atlantic Standard Time
		{-14400, "America/St_Thomas"},                // Atlantic Standard Time
		{-14400, "America/St_Vincent"},               // Atlantic Standard Time
		{-14400, "America/Thule"},                    // Atlantic Standard Time
		{-14400, "America/Tortola"},                  // Atlantic Standard Time
		{-14400, "America/Virgin"},                   // Atlantic Standard Time
		{-14400, "Antarctica/Palmer"},                // Chile Time
		{-14400, "Atlantic/Bermuda"},                 // Atlantic Standard Time
		{-14400, "Atlantic/Stanley"},                 // Falkland Is. Time
		{-14400, "Brazil/Acre"},                      // Amazon Time
		{-14400, "Brazil/West"},                      // Amazon Time
		{-14400, "Canada/Atlantic"},                  // Atlantic Standard Time
		{-14400, "Chile/Continental"},                // Chile Time
		{-14400, "Etc/GMT+4"},                        // GMT-04:00
		{-14400, "PRT"},                              // Atlantic Standard Time
		{-14400, "SystemV/AST4"},                     // Atlantic Standard Time
		{-14400, "SystemV/AST4ADT"},                  // Atlantic Standard Time
		{-12600, "America/St_Johns"},                 // Newfoundland Standard Time
		{-12600, "CNT"},                              // Newfoundland Standard Time
		{-12600, "Canada/Newfoundland"},              // Newfoundland Standard Time
		{-10800, "AGT"},                              // Argentine Time
		{-10800, "America/Araguaina"},                // Brasilia Time
		{-10800, "America/Argentina/Buenos_Aires"},   // Argentine Time
		{-10800, "America/Argentina/Catamarca"},      // Argentine Time
		{-10800, "America/Argentina/ComodRivadavia"}, // Argentine Time
		{-10800, "America/Argentina/Cordoba"},        // Argentine Time
		{-10800, "America/Argentina/Jujuy"},          // Argentine Time
		{-10800, "America/Argentina/La_Rioja"},       // Argentine Time
		{-10800, "America/Argentina/Mendoza"},        // Argentine Time
		{-10800, "America/Argentina/Rio_Gallegos"},   // Argentine Time
		{-10800, "America/Argentina/Salta"},          // Argentine Time
		{-10800, "America/Argentina/San_Juan"},       // Argentine Time
		{-10800, "America/Argentina/Tucuman"},        // Argentine Time
		{-10800, "America/Argentina/Ushuaia"},        // Argentine Time
		{-10800, "America/Bahia"},                    // Brasilia Time
		{-10800, "America/Belem"},                    // Brasilia Time
		{-10800, "America/Buenos_Aires"},             // Argentine Time
		{-10800, "America/Catamarca"},                // Argentine Time
		{-10800, "America/Cayenne"},                  // French Guiana Time
		{-10800, "America/Cordoba"},                  // Argentine Time
		{-10800, "America/Fortaleza"},                // Brasilia Time
		{-10800, "America/Godthab"},                  // Western Greenland Time
		{-10800, "America/Jujuy"},                    // Argentine Time
		{-10800, "America/Maceio"},                   // Brasilia Time
		{-10800, "America/Mendoza"},                  // Argentine Time
		{-10800, "America/Miquelon"},                 // Pierre & Miquelon Standard Time
		{-10800, "America/Montevideo"},               // Uruguay Time
		{-10800, "America/Paramaribo"},               // Suriname Time
		{-10800, "America/Recife"},                   // Brasilia Time
		{-10800, "America/Rosario"},                  // Argentine Time
		{-10800, "America/Santarem"},                 // Brasilia Time
		{-10800, "America/Sao_Paulo"},                // Brasilia Time
		{-10800, "Antarctica/Rothera"},               // Rothera Time
		{-10800, "BET"},                              // Brasilia Time
		{-10800, "Brazil/East"},                      // Brasilia Time
		{-10800, "Etc/GMT+3"},                        // GMT-03:00
		{-7200, "America/Noronha"},                   // Fernando de Noronha Time
		{-7200, "Atlantic/South_Georgia"},            // South Georgia Standard Time
		{-7200, "Brazil/DeNoronha"},                  // Fernando de Noronha Time
		{-7200, "Etc/GMT+2"},                         // GMT-02:00
		{-3600, "America/Scoresbysund"},              // Eastern Greenland Time
		{-3600, "Atlantic/Azores"},                   // Azores Time
		{-3600, "Atlantic/Cape_Verde"},               // Cape Verde Time
		{-3600, "Etc/GMT+1"},                         // GMT-01:00
		{0, "Africa/Abidjan"},                        // Greenwich Mean Time
		{0, "Africa/Accra"},                          // Ghana Mean Time
		{0, "Africa/Bamako"},                         // Greenwich Mean Time
		{0, "Africa/Banjul"},                         // Greenwich Mean Time
		{0, "Africa/Bissau"},                         // Greenwich Mean Time
		{0, "Africa/Casablanca"},                     // Western European Time
		{0, "Africa/Conakry"},                        // Greenwich Mean Time
		{0, "Africa/Dakar"},                          // Greenwich Mean Time
		{0, "Africa/El_Aaiun"},                       // Western European Time
		{0, "Africa/Freetown"},                       // Greenwich Mean Time
		{0, "Africa/Lome"},                           // Greenwich Mean Time
		{0, "Africa/Monrovia"},                       // Greenwich Mean Time
		{0, "Africa/Nouakchott"},                     // Greenwich Mean Time
		{0, "Africa/Ouagadougou"},                    // Greenwich Mean Time
		{0, "Africa/Sao_Tome"},                       // Greenwich Mean Time
		{0, "Africa/Timbuktu"},                       // Greenwich Mean Time
		{0, "America/Danmarkshavn"},                  // Greenwich Mean Time
		{0, "Atlantic/Canary"},                       // Western European Time
		{0, "Atlantic/Faeroe"},                       // Western European Time
		{0, "Atlantic/Faroe"},                        // Western European Time
		{0, "Atlantic/Madeira"},                      // Western European Time
		{0, "Atlantic/Reykjavik"},                    // Greenwich Mean Time
		{0, "Atlantic/St_Helena"},                    // Greenwich Mean Time
		{0, "Eire"},                                  // Greenwich Mean Time
		{0, "Etc/GMT"},                               // GMT+00:00
		{0, "Etc/GMT+0"},                             // GMT+00:00
		{0, "Etc/GMT-0"},                             // GMT+00:00
		{0, "Etc/GMT0"},                              // GMT+00:00
		{0, "Etc/Greenwich"},                         // Greenwich Mean Time
		{0, "Etc/UCT"},                               // Coordinated Universal Time
		{0, "Etc/UTC"},                               // Coordinated Universal Time
		{0, "Etc/Universal"},                         // Coordinated Universal Time
		{0, "Etc/Zulu"},                              // Coordinated Universal Time
		{0, "Europe/Belfast"},                        // Greenwich Mean Time
		{0, "Europe/Dublin"},                         // Greenwich Mean Time
		{0, "Europe/Guernsey"},                       // Greenwich Mean Time
		{0, "Europe/Isle_of_Man"},                    // Greenwich Mean Time
		{0, "Europe/Jersey"},                         // Greenwich Mean Time
		{0, "Europe/Lisbon"},                         // Western European Time
		{0, "Europe/London"},                         // Greenwich Mean Time
		{0, "GB"},                                    // Greenwich Mean Time
		{0, "GB-Eire"},                               // Greenwich Mean Time
		{0, "GMT"},                                   // Greenwich Mean Time
		{0, "GMT0"},                                  // GMT+00:00
		{0, "Greenwich"},                             // Greenwich Mean Time
		{0, "Iceland"},                               // Greenwich Mean Time
		{0, "Portugal"},                              // Western European Time
		{0, "UCT"},                                   // Coordinated Universal Time
		{0, "UTC"},                                   // Coordinated Universal Time
		{0, "Universal"},                             // Coordinated Universal Time
		{0, "WET"},                                   // Western European Time
		{0, "Zulu"},                                  // Coordinated Universal Time
		{3600, "Africa/Algiers"},                     // Central European Time
		{3600, "Africa/Bangui"},                      // Western African Time
		{3600, "Africa/Brazzaville"},                 // Western African Time
		{3600, "Africa/Ceuta"},                       // Central European Time
		{3600, "Africa/Douala"},                      // Western African Time
		{3600, "Africa/Kinshasa"},                    // Western African Time
		{3600, "Africa/Lagos"},                       // Western African Time
		{3600, "Africa/Libreville"},                  // Western African Time
		{3600, "Africa/Luanda"},                      // Western African Time
		{3600, "Africa/Malabo"},                      // Western African Time
		{3600, "Africa/Ndjamena"},                    // Western African Time
		{3600, "Africa/Niamey"},                      // Western African Time
		{3600, "Africa/Porto-Novo"},                  // Western African Time
		{3600, "Africa/Tunis"},                       // Central European Time
		{3600, "Africa/Windhoek"},                    // Western African Time
		{3600, "Arctic/Longyearbyen"},                // Central European Time
		{3600, "Atlantic/Jan_Mayen"},                 // Central European Time
		{3600, "CET"},                                // Central European Time
		{3600, "ECT"},                                // Central European Time
		{3600, "Etc/GMT-1"},                          // GMT+01:00
		{3600, "Europe/Amsterdam"},                   // Central European Time
		{3600, "Europe/Andorra"},                     // Central European Time
		{3600, "Europe/Belgrade"},                    // Central European Time
		{3600, "Europe/Berlin"},                      // Central European Time
		{3600, "Europe/Bratislava"},                  // Central European Time
		{3600, "Europe/Brussels"},                    // Central European Time
		{3600, "Europe/Budapest"},                    // Central European Time
		{3600, "Europe/Copenhagen"},                  // Central European Time
		{3600, "Europe/Gibraltar"},                   // Central European Time
		{3600, "Europe/Ljubljana"},                   // Central European Time
		{3600, "Europe/Luxembourg"},                  // Central European Time
		{3600, "Europe/Madrid"},                      // Central European Time
		{3600, "Europe/Malta"},                       // Central European Time
		{3600, "Europe/Monaco"},                      // Central European Time
		{3600, "Europe/Oslo"},                        // Central European Time
		{3600, "Europe/Paris"},                       // Central European Time
		{3600, "Europe/Podgorica"},                   // Central European Time
		{3600, "Europe/Prague"},                      // Central European Time
		{3600, "Europe/Rome"},                        // Central European Time
		{3600, "Europe/San_Marino"},                  // Central European Time
		{3600, "Europe/Sarajevo"},                    // Central European Time
		{3600, "Europe/Skopje"},                      // Central European Time
		{3600, "Europe/Stockholm"},                   // Central European Time
		{3600, "Europe/Tirane"},                      // Central European Time
		{3600, "Europe/Vaduz"},                       // Central European Time
		{3600, "Europe/Vatican"},                     // Central European Time
		{3600, "Europe/Vienna"},                      // Central European Time
		{3600, "Europe/Warsaw"},                      // Central European Time
		{3600, "Europe/Zagreb"},                      // Central European Time
		{3600, "Europe/Zurich"},                      // Central European Time
		{3600, "MET"},                                // Middle Europe Time
		{3600, "Poland"},                             // Central European Time
		{7200, "ART"},                                // Eastern European Time
		{7200, "Africa/Blantyre"},                    // Central African Time
		{7200, "Africa/Bujumbura"},                   // Central African Time
		{7200, "Africa/Cairo"},                       // Eastern European Time
		{7200, "Africa/Gaborone"},                    // Central African Time
		{7200, "Africa/Harare"},                      // Central African Time
		{7200, "Africa/Johannesburg"},                // South Africa Standard Time
		{7200, "Africa/Kigali"},                      // Central African Time
		{7200, "Africa/Lubumbashi"},                  // Central African Time
		{7200, "Africa/Lusaka"},                      // Central African Time
		{7200, "Africa/Maputo"},                      // Central African Time
		{7200, "Africa/Maseru"},                      // South Africa Standard Time
		{7200, "Africa/Mbabane"},                     // South Africa Standard Time
		{7200, "Africa/Tripoli"},                     // Eastern European Time
		{7200, "Asia/Amman"},                         // Eastern European Time
		{7200, "Asia/Beirut"},                        // Eastern European Time
		{7200, "Asia/Damascus"},                      // Eastern European Time
		{7200, "Asia/Gaza"},                          // Eastern European Time
		{7200, "Asia/Hebron"},                        // GMT+02:00
		{7200, "Asia/Istanbul"},                      // Eastern European Time
		{7200, "Asia/Jerusalem"},                     // Israel Standard Time
		{7200, "Asia/Nicosia"},                       // Eastern European Time
		{7200, "Asia/Tel_Aviv"},                      // Israel Standard Time
		{7200, "CAT"},                                // Central African Time
		{7200, "EET"},                                // Eastern European Time
		{7200, "Egypt"},                              // Eastern European Time
		{7200, "Etc/GMT-2"},                          // GMT+02:00
		{7200, "Europe/Athens"},                      // Eastern European Time
		{7200, "Europe/Bucharest"},                   // Eastern European Time
		{7200, "Europe/Chisinau"},                    // Eastern European Time
		{7200, "Europe/Helsinki"},                    // Eastern European Time
		{7200, "Europe/Istanbul"},                    // Eastern European Time
		{7200, "Europe/Kiev"},                        // Eastern European Time
		{7200, "Europe/Mariehamn"},                   // Eastern European Time
		{7200, "Europe/Nicosia"},                     // Eastern European Time
		{7200, "Europe/Riga"},                        // Eastern European Time
		{7200, "Europe/Simferopol"},                  // Eastern European Time
		{7200, "Europe/Sofia"},                       // Eastern European Time
		{7200, "Europe/Tallinn"},                     // Eastern European Time
		{7200, "Europe/Tiraspol"},                    // Eastern European Time
		{7200, "Europe/Uzhgorod"},                    // Eastern European Time
		{7200, "Europe/Vilnius"},                     // Eastern European Time
		{7200, "Europe/Zaporozhye"},                  // Eastern European Time
		{7200, "Israel"},                             // Israel Standard Time
		{7200, "Libya"},                              // Eastern European Time
		{7200, "Turkey"},                             // Eastern European Time
		{10800, "Africa/Addis_Ababa"},                // Eastern African Time
		{10800, "Africa/Asmara"},                     // Eastern African Time
		{10800, "Africa/Asmera"},                     // Eastern African Time
		{10800, "Africa/Dar_es_Salaam"},              // Eastern African Time
		{10800, "Africa/Djibouti"},                   // Eastern African Time
		{10800, "Africa/Juba"},                       // GMT+03:00
		{10800, "Africa/Kampala"},                    // Eastern African Time
		{10800, "Africa/Khartoum"},                   // Eastern African Time
		{10800, "Africa/Mogadishu"},                  // Eastern African Time
		{10800, "Africa/Nairobi"},                    // Eastern African Time
		{10800, "Antarctica/Syowa"},                  // Syowa Time
		{10800, "Asia/Aden"},                         // Arabia Standard Time
		{10800, "Asia/Baghdad"},                      // Arabia Standard Time
		{10800, "Asia/Bahrain"},                      // Arabia Standard Time
		{10800, "Asia/Kuwait"},                       // Arabia Standard Time
		{10800, "Asia/Qatar"},                        // Arabia Standard Time
		{10800, "Asia/Riyadh"},                       // Arabia Standard Time
		{10800, "EAT"},                               // Eastern African Time
		{10800, "Etc/GMT-3"},                         // GMT+03:00
		{10800, "Europe/Kaliningrad"},                // Eastern European Time
		{10800, "Europe/Minsk"},                      // Eastern European Time
		{10800, "Indian/Antananarivo"},               // Eastern African Time
		{10800, "Indian/Comoro"},                     // Eastern African Time
		{10800, "Indian/Mayotte"},                    // Eastern African Time
		{11224, "Asia/Riyadh87"},                     // GMT+03:07
		{11224, "Asia/Riyadh88"},                     // GMT+03:07
		{11224, "Asia/Riyadh89"},                     // GMT+03:07
		{11224, "Mideast/Riyadh87"},                  // GMT+03:07
		{11224, "Mideast/Riyadh88"},                  // GMT+03:07
		{11224, "Mideast/Riyadh89"},                  // GMT+03:07
		{12600, "Asia/Tehran"},                       // Iran Standard Time
		{12600, "Iran"},                              // Iran Standard Time
		{14400, "Asia/Baku"},                         // Azerbaijan Time
		{14400, "Asia/Dubai"},                        // Gulf Standard Time
		{14400, "Asia/Muscat"},                       // Gulf Standard Time
		{14400, "Asia/Tbilisi"},                      // Georgia Time
		{14400, "Asia/Yerevan"},                      // Armenia Time
		{14400, "Etc/GMT-4"},                         // GMT+04:00
		{14400, "Europe/Moscow"},                     // Moscow Standard Time
		{14400, "Europe/Samara"},                     // Samara Time
		{14400, "Europe/Volgograd"},                  // Volgograd Time
		{14400, "Indian/Mahe"},                       // Seychelles Time
		{14400, "Indian/Mauritius"},                  // Mauritius Time
		{14400, "Indian/Reunion"},                    // Reunion Time
		{14400, "NET"},                               // Armenia Time
		{14400, "W-SU"},                              // Moscow Standard Time
		{16200, "Asia/Kabul"},                        // Afghanistan Time
		{18000, "Antarctica/Mawson"},                 // Mawson Time
		{18000, "Asia/Aqtau"},                        // Aqtau Time
		{18000, "Asia/Aqtobe"},                       // Aqtobe Time
		{18000, "Asia/Ashgabat"},                     // Turkmenistan Time
		{18000, "Asia/Ashkhabad"},                    // Turkmenistan Time
		{18000, "Asia/Dushanbe"},                     // Tajikistan Time
		{18000, "Asia/Karachi"},                      // Pakistan Time
		{18000, "Asia/Oral"},                         // Oral Time
		{18000, "Asia/Samarkand"},                    // Uzbekistan Time
		{18000, "Asia/Tashkent"},                     // Uzbekistan Time
		{18000, "Etc/GMT-5"},                         // GMT+05:00
		{18000, "Indian/Kerguelen"},                  // French Southern & Antarctic Lands Time
		{18000, "Indian/Maldives"},                   // Maldives Time
		{18000, "PLT"},                               // Pakistan Time
		{19800, "Asia/Calcutta"},                     // India Standard Time
		{19800, "Asia/Colombo"},                      // India Standard Time
		{19800, "Asia/Kolkata"},                      // India Standard Time
		{19800, "IST"},                               // India Standard Time
		{20700, "Asia/Kathmandu"},                    // Nepal Time
		{20700, "Asia/Katmandu"},                     // Nepal Time
		{21600, "Antarctica/Vostok"},                 // Vostok Time
		{21600, "Asia/Almaty"},                       // Alma-Ata Time
		{21600, "Asia/Bishkek"},                      // Kirgizstan Time
		{21600, "Asia/Dacca"},                        // Bangladesh Time
		{21600, "Asia/Dhaka"},                        // Bangladesh Time
		{21600, "Asia/Qyzylorda"},                    // Qyzylorda Time
		{21600, "Asia/Thimbu"},                       // Bhutan Time
		{21600, "Asia/Thimphu"},                      // Bhutan Time
		{21600, "Asia/Yekaterinburg"},                // Yekaterinburg Time
		{21600, "BST"},                               // Bangladesh Time
		{21600, "Etc/GMT-6"},                         // GMT+06:00
		{21600, "Indian/Chagos"},                     // Indian Ocean Territory Time
		{23400, "Asia/Rangoon"},                      // Myanmar Time
		{23400, "Indian/Cocos"},                      // Cocos Islands Time
		{25200, "Antarctica/Davis"},                  // Davis Time
		{25200, "Asia/Bangkok"},                      // Indochina Time
		{25200, "Asia/Ho_Chi_Minh"},                  // Indochina Time
		{25200, "Asia/Hovd"},                         // Hovd Time
		{25200, "Asia/Jakarta"},                      // West Indonesia Time
		{25200, "Asia/Novokuznetsk"},                 // Novosibirsk Time
		{25200, "Asia/Novosibirsk"},                  // Novosibirsk Time
		{25200, "Asia/Omsk"},                         // Omsk Time
		{25200, "Asia/Phnom_Penh"},                   // Indochina Time
		{25200, "Asia/Pontianak"},                    // West Indonesia Time
		{25200, "Asia/Saigon"},                       // Indochina Time
		{25200, "Asia/Vientiane"},                    // Indochina Time
		{25200, "Etc/GMT-7"},                         // GMT+07:00
		{25200, "Indian/Christmas"},                  // Christmas Island Time
		{25200, "VST"},                               // Indochina Time
		{28800, "Antarctica/Casey"},                  // Western Standard Time (Australia)
		{28800, "Asia/Brunei"},                       // Brunei Time
		{28800, "Asia/Choibalsan"},                   // Choibalsan Time
		{28800, "Asia/Chongqing"},                    // China Standard Time
		{28800, "Asia/Chungking"},                    // China Standard Time
		{28800, "Asia/Harbin"},                       // China Standard Time
		{28800, "Asia/Hong_Kong"},                    // Hong Kong Time
		{28800, "Asia/Kashgar"},                      // China Standard Time
		{28800, "Asia/Krasnoyarsk"},                  // Krasnoyarsk Time
		{28800, "Asia/Kuala_Lumpur"},                 // Malaysia Time
		{28800, "Asia/Kuching"},                      // Malaysia Time
		{28800, "Asia/Macao"},                        // China Standard Time
		{28800, "Asia/Macau"},                        // China Standard Time
		{28800, "Asia/Makassar"},                     // Central Indonesia Time
		{28800, "Asia/Manila"},                       // Philippines Time
		{28800, "Asia/Shanghai"},                     // China Standard Time
		{28800, "Asia/Singapore"},                    // Singapore Time
		{28800, "Asia/Taipei"},                       // China Standard Time
		{28800, "Asia/Ujung_Pandang"},                // Central Indonesia Time
		{28800, "Asia/Ulaanbaatar"},                  // Ulaanbaatar Time
		{28800, "Asia/Ulan_Bator"},                   // Ulaanbaatar Time
		{28800, "Asia/Urumqi"},                       // China Standard Time
		{28800, "Australia/Perth"},                   // Western Standard Time (Australia)
		{28800, "Australia/West"},                    // Western Standard Time (Australia)
		{28800, "CTT"},                               // China Standard Time
		{28800, "Etc/GMT-8"},                         // GMT+08:00
		{28800, "Hongkong"},                          // Hong Kong Time
		{28800, "PRC"},                               // China Standard Time
		{28800, "Singapore"},                         // Singapore Time
		{31500, "Australia/Eucla"},                   // Central Western Standard Time (Australia)
		{32400, "Asia/Dili"},                         // Timor-Leste Time
		{32400, "Asia/Irkutsk"},                      // Irkutsk Time
		{32400, "Asia/Jayapura"},                     // East Indonesia Time
		{32400, "Asia/Pyongyang"},                    // Korea Standard Time
		{32400, "Asia/Seoul"},                        // Korea Standard Time
		{32400, "Asia/Tokyo"},                        // Japan Standard Time
		{32400, "Etc/GMT-9"},                         // GMT+09:00
		{32400, "JST"},                               // Japan Standard Time
		{32400, "Japan"},                             // Japan Standard Time
		{32400, "Pacific/Palau"},                     // Palau Time
		{32400, "ROK"},                               // Korea Standard Time
		{34200, "ACT"},                               // Central Standard Time (Northern Territory)
		{34200, "Australia/Adelaide"},                // Central Standard Time (South Australia)
		{34200, "Australia/Broken_Hill"},             // Central Standard Time (South Australia/New South Wales)
		{34200, "Australia/Darwin"},                  // Central Standard Time (Northern Territory)
		{34200, "Australia/North"},                   // Central Standard Time (Northern Territory)
		{34200, "Australia/South"},                   // Central Standard Time (South Australia)
		{34200, "Australia/Yancowinna"},              // Central Standard Time (South Australia/New South Wales)
		{36000, "AET"},                               // Eastern Standard Time (New South Wales)
		{36000, "Antarctica/DumontDUrville"},         // Dumont-d\'Urville Time
		{36000, "Asia/Yakutsk"},                      // Yakutsk Time
		{36000, "Australia/ACT"},                     // Eastern Standard Time (New South Wales)
		{36000, "Australia/Brisbane"},                // Eastern Standard Time (Queensland)
		{36000, "Australia/Canberra"},                // Eastern Standard Time (New South Wales)
		{36000, "Australia/Currie"},                  // Eastern Standard Time (New South Wales)
		{36000, "Australia/Hobart"},                  // Eastern Standard Time (Tasmania)
		{36000, "Australia/Lindeman"},                // Eastern Standard Time (Queensland)
		{36000, "Australia/Melbourne"},               // Eastern Standard Time (Victoria)
		{36000, "Australia/NSW"},                     // Eastern Standard Time (New South Wales)
		{36000, "Australia/Queensland"},              // Eastern Standard Time (Queensland)
		{36000, "Australia/Sydney"},                  // Eastern Standard Time (New South Wales)
		{36000, "Australia/Tasmania"},                // Eastern Standard Time (Tasmania)
		{36000, "Australia/Victoria"},                // Eastern Standard Time (Victoria)
		{36000, "Etc/GMT-10"},                        // GMT+10:00
		{36000, "Pacific/Chuuk"},                     // Chuuk Time
		{36000, "Pacific/Guam"},                      // Chamorro Standard Time
		{36000, "Pacific/Port_Moresby"},              // Papua New Guinea Time
		{36000, "Pacific/Saipan"},                    // Chamorro Standard Time
		{36000, "Pacific/Truk"},                      // Chuuk Time
		{36000, "Pacific/Yap"},                       // Chuuk Time
		{37800, "Australia/LHI"},                     // Lord Howe Standard Time
		{37800, "Australia/Lord_Howe"},               // Lord Howe Standard Time
		{39600, "Antarctica/Macquarie"},              // Macquarie Island Time
		{39600, "Asia/Sakhalin"},                     // Sakhalin Time
		{39600, "Asia/Vladivostok"},                  // Vladivostok Time
		{39600, "Etc/GMT-11"},                        // GMT+11:00
		{39600, "Pacific/Efate"},                     // Vanuatu Time
		{39600, "Pacific/Guadalcanal"},               // Solomon Is. Time
		{39600, "Pacific/Kosrae"},                    // Kosrae Time
		{39600, "Pacific/Noumea"},                    // New Caledonia Time
		{39600, "Pacific/Pohnpei"},                   // Pohnpei Time
		{39600, "Pacific/Ponape"},                    // Pohnpei Time
		{39600, "SST"},                               // Solomon Is. Time
		{41400, "Pacific/Norfolk"},                   // Norfolk Time
		{43200, "Antarctica/McMurdo"},                // New Zealand Standard Time
		{43200, "Antarctica/South_Pole"},             // New Zealand Standard Time
		{43200, "Asia/Anadyr"},                       // Anadyr Time
		{43200, "Asia/Kamchatka"},                    // Petropavlovsk-Kamchatski Time
		{43200, "Asia/Magadan"},                      // Magadan Time
		{43200, "Etc/GMT-12"},                        // GMT+12:00
		{43200, "Kwajalein"},                         // Marshall Islands Time
		{43200, "NST"},                               // New Zealand Standard Time
		{43200, "NZ"},                                // New Zealand Standard Time
		{43200, "Pacific/Auckland"},                  // New Zealand Standard Time
		{43200, "Pacific/Fiji"},                      // Fiji Time
		{43200, "Pacific/Funafuti"},                  // Tuvalu Time
		{43200, "Pacific/Kwajalein"},                 // Marshall Islands Time
		{43200, "Pacific/Majuro"},                    // Marshall Islands Time
		{43200, "Pacific/Nauru"},                     // Nauru Time
		{43200, "Pacific/Tarawa"},                    // Gilbert Is. Time
		{43200, "Pacific/Wake"},                      // Wake Time
		{43200, "Pacific/Wallis"},                    // Wallis & Futuna Time
		{45900, "NZ-CHAT"},                           // Chatham Standard Time
		{45900, "Pacific/Chatham"},                   // Chatham Standard Time
		{46800, "Etc/GMT-13"},                        // GMT+13:00
		{46800, "MIT"},                               // West Samoa Time
		{46800, "Pacific/Apia"},                      // West Samoa Time
		{46800, "Pacific/Enderbury"},                 // Phoenix Is. Time
		{46800, "Pacific/Tongatapu"},                 // Tonga Time
		{50400, "Etc/GMT-14"},                        // GMT+14:00
		{50400, "Pacific/Kiritimati"}                 // Line Is. Time
});

__INTERNAL_END__


#endif // WASP_UTILS__DATETIME_CONSTANTS_H
