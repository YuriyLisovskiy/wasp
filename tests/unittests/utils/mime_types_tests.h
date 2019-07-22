/*
 * Copyright (c) 2019 Yuriy Lisovskiy
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

#ifndef WASP_UNIT_TESTS_MIME_TYPES_TESTS_H
#define WASP_UNIT_TESTS_MIME_TYPES_TESTS_H

#include <gtest/gtest.h>

#include "../../../src/globals.h"
#include "../../../src/utils/mime_types.h"


TEST(MimeTypesTest, getExtFromFileNameTestSuccess)
{
	ASSERT_EQ(wasp::mime::internal::getExtFromFileName("foo.bar"), "bar");
	ASSERT_EQ(wasp::mime::internal::getExtFromFileName(".bar"), "bar");
}

TEST(MimeTypesTest, getExtFromFileNameTestNoExtension)
{
	ASSERT_EQ(wasp::mime::internal::getExtFromFileName("foo"), "");
	ASSERT_EQ(wasp::mime::internal::getExtFromFileName("bar"), "");
}

TEST(MimeTypesTest, getExtFromFileNameTestEmptyExtension)
{
	ASSERT_EQ(wasp::mime::internal::getExtFromFileName("foo."), "");
	ASSERT_EQ(wasp::mime::internal::getExtFromFileName("bar."), "");
}

TEST(MimeTypesTest, getExtensionFromPathTestSuccess)
{
	ASSERT_EQ(wasp::mime::getExtensionFromPath("foo.bar"), "bar");
	ASSERT_EQ(wasp::mime::getExtensionFromPath("path/to/foo.bar"), "bar");
	ASSERT_EQ(wasp::mime::getExtensionFromPath("/foo.bar"), "bar");
	ASSERT_EQ(wasp::mime::getExtensionFromPath("./foo.bar"), "bar");
	ASSERT_EQ(wasp::mime::getExtensionFromPath("./.foo.bar"), "bar");
}

TEST(MimeTypesTest, getExtensionFromPathTestNoExtension)
{
	ASSERT_EQ(wasp::mime::getExtensionFromPath("foo"), "");
	ASSERT_EQ(wasp::mime::getExtensionFromPath("path/to/foo"), "");
	ASSERT_EQ(wasp::mime::getExtensionFromPath("/foo"), "");
	ASSERT_EQ(wasp::mime::getExtensionFromPath("./foo"), "");
}

TEST(MimeTypesTest, getExtensionFromPathTestEmptyExtension)
{
	ASSERT_EQ(wasp::mime::getExtensionFromPath("foo."), "");
	ASSERT_EQ(wasp::mime::getExtensionFromPath("path/to/foo."), "");
	ASSERT_EQ(wasp::mime::getExtensionFromPath("/foo."), "");
	ASSERT_EQ(wasp::mime::getExtensionFromPath("./foo."), "");
	ASSERT_EQ(wasp::mime::getExtensionFromPath("./.foo."), "");
	ASSERT_EQ(wasp::mime::getExtensionFromPath("../.foo."), "");
}

#endif // WASP_UNIT_TESTS_MIME_TYPES_TESTS_H
