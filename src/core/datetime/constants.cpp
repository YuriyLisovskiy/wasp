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
 * An implementation of constants.h.
 */

#include "./constants.h"


__DATETIME_INTERNAL_BEGIN__

collections::Dict<std::string, int> TZ_TO_OFFSET(std::map<std::string, int> {
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

collections::Dict<int, std::string> OFFSET_TO_TZ_ABBR(std::map<int, std::string>
{
	{0, "GMT"},
	{7200, "EET"},
	{10800, "EEDT"},
	// TODO: add data to OFFSET_TO_TZ_ABBR
});

collections::Dict<std::string, int> TZ_ABBR_TO_OFFSET(std::map<std::string, int>
{
	{"GMT", 0},
	{"EET", 7200},
	{"EEDT", 10800},
	// TODO: add data to TZ_ABBR_TO_OFFSET
});

__DATETIME_INTERNAL_END__
