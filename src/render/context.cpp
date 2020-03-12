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
 * An implementation of render/context.h
 */

#include "./context.h"


__RENDER_BEGIN__

Context::Context(
	std::map<std::string, std::shared_ptr<core::object::Object>> global_scope,
	bool auto_delete
) : _auto_delete(auto_delete), _global_scope(std::move(global_scope))
{
}

Context::~Context()
{
	// TODO: ~Context(): check why segmentation fault is thrown after template is rendered
//	if (this->_auto_delete)
//	{
//		for (auto& var : this->_global_scope)
//		{
//			delete var.second;
//		}
//	}
}

std::shared_ptr<core::object::Object> Context::find_var(const std::string& key)
{
	auto var_p = this->_global_scope.find(key);
	if (var_p != this->_global_scope.end())
	{
		return var_p->second;
	}

	return nullptr;
}

__RENDER_END__
