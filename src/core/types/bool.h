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
 * types/bool.h
 *
 * Purpose:
 * TODO: write docs for types/bool.h
 */

#pragma once

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "../object/object.h"


__TYPES_BEGIN__

class Bool final : public object::Object
{
private:
	bool _val;

public:
	explicit Bool(bool val);
	[[nodiscard]] std::string to_string() override;
	[[nodiscard]] int compare_to(const Object& other) const override;
};

__TYPES_END__
