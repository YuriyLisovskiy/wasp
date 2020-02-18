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
 * An implementation of render/processors/base.h
 */

#include "./base.h"


__RENDER_INTERNAL_BEGIN__

Variable::Variable() : _content("")
{
}

Variable::Variable(
	const std::string& content,
	const std::vector<std::string>& attributes
)
{
	this->_content = content;
}

core::object::Object* Variable::resolve(IContext* ctx)
{
	// TODO: implement Variable::resolve(IContext* ctx)
	return nullptr;
}

__RENDER_INTERNAL_END__
