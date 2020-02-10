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
 * An implementation of types/bool.h
 */

#include "./bool.h"


__TYPES_BEGIN__

Bool::Bool(bool val)
{
	this->_val = val;
}

std::string Bool::to_string()
{
	return this->_val ? "true" : "false";
}

int Bool::compare_to(const Object& other) const
{
	// TODO: implement int Bool::compare_to(const Object& other) const
	return 0;
}

__TYPES_END__
