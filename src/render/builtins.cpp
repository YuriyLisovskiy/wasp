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
 * An implementation of render/builtins.h
 */

#include "./builtins.h"


__RENDER_BEGIN__

// TODO: implement default filters.
collections::Dict<std::string, Filter> DEFAULT_FILTERS(
	std::map<std::string, Filter>{

		// TODO: temporary filter for testing
		{"append_sym", [](const std::shared_ptr<core::object::Object>& obj, const KwArgs& kwargs) -> std::shared_ptr<core::object::Object>{
		//	*(obj->__cast__<core::types::Value<std::string>>()) += kwargs.get("sym")->__str__();

			std::string sym = kwargs.get("sym")->__str__();
			return std::shared_ptr<core::object::Object>(new core::types::Value<std::string>(
				*(obj->__cast__<core::types::Value<std::string>>()) + sym
			));
		}}
	},
	true
);

__RENDER_END__
