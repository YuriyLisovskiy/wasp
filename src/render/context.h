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
#include <memory>

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "./base.h"
#include "../core/object/object.h"


__RENDER_BEGIN__

class Context : public IContext
{
private:
	std::map<std::string, std::shared_ptr<core::object::Object>> _global_scope;
	std::map<std::string, std::shared_ptr<core::object::Object>> _local_scope;

protected:
	static std::shared_ptr<core::object::Object> find_in_scope(
		std::map<std::string, std::shared_ptr<core::object::Object>>& scope,
		const std::string& key
	);

public:
	explicit Context(
		std::map<std::string, std::shared_ptr<core::object::Object>> global_scope
	);
	std::shared_ptr<core::object::Object> find_var(const std::string& key) override;
	void push_var(
		const std::string& key,
		const std::shared_ptr<core::object::Object>& val
	) override;
};

__RENDER_END__
