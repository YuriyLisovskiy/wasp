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

// Framework modules.
#include "../../core/types/value.h"
#include "../../core/strings.h"


__RENDER_INTERNAL_BEGIN__

Variable::Variable() : _content(""), _is_constant(true)
{
}

std::regex Variable::number_regex = std::regex(R"((\-|\+?)?\d+(\.\d+(f)?)?(l)?)");

Variable::Variable(
	const std::string& content,
	const std::vector<std::string>& attributes,
	bool is_constant
)
{
	this->_content = content;
	this->_is_constant = is_constant;
}

std::shared_ptr<core::object::Object> Variable::resolve(
	IContext* ctx
)
{
	if (this->_content.empty())
	{
		return nullptr;
	}
	else if (this->_is_constant)
	{
		std::shared_ptr<core::object::Object> const_obj;
		auto c = this->_content;
		std::smatch sm;
		if (std::regex_match(c, sm, Variable::number_regex))
		{
			bool is_int = sm[2].str().empty();
			if (is_int)
			{
				if (sm[4].str().empty())
				{
					const_obj = std::make_shared<core::types::Value<int>>(std::stoi(sm[0]));
				}
				else
				{
					const_obj = std::make_shared<core::types::Value<long>>(std::stol(sm[0]));
				}
			}
			else
			{
				if (sm[3].str().empty())
				{
					const_obj = std::make_shared<core::types::Value<double>>(std::stod(sm[0]));
				}
				else
				{
					const_obj = std::make_shared<core::types::Value<float>>(std::stof(sm[0]));
				}
			}
		}
		else
		{
			core::str::trim(c, "'");
			core::str::trim(c, "\"");
			const_obj = std::make_shared<core::types::Value<std::string>>(c);
		}

		return const_obj;
	}
	else
	{
		if (!ctx)
		{
			return nullptr;
		}

		return ctx->find_var(this->_content);
	}
}

__RENDER_INTERNAL_END__
