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

#pragma once

// Module definitions.
#include "./_def_.h"


__CONF_INTERNAL_BEGIN__

const char* const STARTUP_MESSAGE = "Wasp version 0.0.1-dev, using app settings"
									"\nStarting development server at {0!s}://{1!s}:{2!d}/"
		                            "\nQuit the server with CONTROL-C.";

__CONF_INTERNAL_END__
