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
 * _def_.h
 * Purpose: main module's definitions.
 */

#pragma once


// xw
#define __MAIN_NAMESPACE_BEGIN__ namespace xw {
#define __MAIN_NAMESPACE_END__ }


__MAIN_NAMESPACE_BEGIN__

const char* const LIB_NAME = "Xalwart";
const char* const LIB_VERSION = "0.0.1-dev";

__MAIN_NAMESPACE_END__


// Required parameters for built-in logger.
#define _ERROR_DETAILS_ __LINE__, __PRETTY_FUNCTION__, __FILE__
