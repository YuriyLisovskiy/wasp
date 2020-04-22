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

Context::Context(const scope_t& global_scope)
{
	this->_scopes.push_back(global_scope);
}

std::shared_ptr<core::object::Object> Context::find_in_scope(
	scope_t& scope, const std::string& key
)
{
	auto var_p = scope.find(key);
	if (var_p != scope.end())
	{
		return var_p->second;
	}

	return nullptr;
}

std::shared_ptr<core::object::Object> Context::find_var(const std::string& key)
{
	for (long i = (long)this->_scopes.size() - 1; i >= 0; i--)
	{
		auto var_p = Context::find_in_scope(this->_scopes[i], key);
		if (var_p)
		{
			return var_p;
		}
	}

	return nullptr;
}

void Context::push_var(
	const std::string& key,
	const std::shared_ptr<core::object::Object>& val
)
{
	this->_scopes[this->_scopes.size() - 1][key] = val;
}

void Context::push_scope(const scope_t& scope)
{
	this->_scopes.push_back(scope);
}

void Context::pop_scope()
{
	if (this->_scopes.size() == 1)
	{
		throw core::RuntimeError("unable to pop global scope");
	}

	this->_scopes.pop_back();
}

__RENDER_END__
