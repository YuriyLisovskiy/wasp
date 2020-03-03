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
 * render/context.h
 *
 * Purpose: main context for Template.
 */

#pragma once

// C++ libraries.
#include <string>
#include <map>

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "./base.h"
#include "../core/object/object.h"


__RENDER_BEGIN__

class Context : public IContext
{
	// TODO: implement Context.
private:
	bool _auto_delete;
	const std::map<std::string, core::object::Object*>* _global_scope;

public:
	explicit Context(
		const std::map<std::string, core::object::Object*>& global_scope,
		bool auto_delete = true
	);
	~Context() override;
	core::object::Object* find_var(const std::string& key) override;
};

__RENDER_END__
