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
 * defaults.h
 *
 * Default configuration constants.
 */

#ifndef WASP_CONF_DEFAULTS_H
#define WASP_CONF_DEFAULTS_H

// Module definitions.
#include "./_def_.h"


__CONF_INTERNAL_BEGIN__

// Local host.
const char* const DEFAULT_HOST = "127.0.0.1";

const uint16_t DEFAULT_PORT = 8000;

const char* const STARTUP_MESSAGE = "Wasp version 0.0.1-dev, using app settings"
									"\nStarting development server at {0!s}://{1!s}:{2!d}/"
		                            "\nQuit the server with CONTROL-C.";

__CONF_INTERNAL_END__


#endif // WASP_CONF_DEFAULTS_H
