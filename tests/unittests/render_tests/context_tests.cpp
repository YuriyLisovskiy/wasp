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

#include <memory>

#include <gtest/gtest.h>

#include "../_def_.h"
#include "../../../src/render/context.h"
#include "../../../src/core/types/value.h"


__UNIT_TESTS_BEGIN__

// TODO: remove test
TEST(ConextTestCase, check)
{
	auto* ctx = new render::Context({
		{"", std::make_shared<core::types::Value<int>>(10)}
	});
	delete ctx;
}

__UNIT_TESTS_END__
