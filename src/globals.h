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
 * This header contains global variables, constants and definitions.
 */

#ifndef WASP_MACRO_H
#define WASP_MACRO_H

#define __WASP_BEGIN__ namespace wasp {
#define __WASP_END__ }

#define __INTERNAL_BEGIN__ __WASP_BEGIN__ namespace internal {
#define __INTERNAL_END__ } __WASP_END__

#define _ERROR_DETAILS_ __LINE__, __FUNCTION__, __FILE__

#endif // WASP_MACRO_H
