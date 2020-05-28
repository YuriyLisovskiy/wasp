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
 * http/status.h
 *
 * Purpose:
 * 	Hold all available http statuses, which contains status code,
 * 	reason phrase and brief explanation.
 */

#pragma once

#ifdef _MSC_VER
#include <string>
#endif

// Module definitions.
#include "./_def_.h"

// Framework modules.
#include "../collections/dict.h"


__HTTP_INTERNAL_BEGIN__

extern collections::Dict<unsigned short int, std::pair<std::string, std::string>> HTTP_STATUS;

__HTTP_INTERNAL_END__
