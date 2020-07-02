/*
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
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
 * core/path.h
 *
 * Purpose: utilities for working with path.
 */

#pragma once

// C++ libraries.
#include <string>
#include <vector>

// Module definitions.
#include "./_def_.h"


__PATH_BEGIN__

/// Split a path in root and extension.
/// The extension is everything starting at the last dot in the last
/// pathname component; the root is everything before that.
/// It is always true that root + ext == p.
extern void split_text(const std::string& full_path, std::string& root_out, std::string& ext_out);

/// Checks if path exists.
///
/// @param path: path to check.
/// @return true if path exists, otherwise returns false.
extern bool exists(const std::string& path);

/// Returns base directory name from given path.
///
/// @param path: path to analyze.
/// @return base of given path.
extern std::string dirname(const std::string& path);

/// Returns file name from given path.
///
/// @param path: path to analyze.
/// @return file name of given path.
extern std::string basename(const std::string& path);

/// Returns file size in bytes.
///
/// @param path: path to access.
/// @return size of file located on given path.
extern size_t get_size(const std::string& path);

/// Joins two strings to single path.
///
/// @param left: left path part.
/// @param right: right path part.
/// @return concatenated single path.
extern std::string join(const std::string& left, const std::string& right);

/// Joins vector of strings to single path.
///
/// @param paths: vector of path parts.
/// @return concatenated single path.
extern std::string join(const std::vector<std::string>& paths);

/// Returns current working directory.
extern std::string cwd();

/// Checks if path is absolute or not.
///
/// @param p: path to check.
/// @return true if path is absolute, otherwise returns false.
extern bool is_absolute(const std::string& p);

__PATH_END__


__PATH_INTERNAL_BEGIN__

/// Split a path in root and extension.
/// The extension is everything starting at the last dot in the last
/// pathname component; the root is everything before that.
/// It is always true that root + ext == p.
///
/// Generic implementation of splitext, to be parametrized with
/// the separators.
extern void _split_text(
	const std::string& full_path,
	char sep,
	char altsep,
	char extsep,
	std::string& root_out,
	std::string& ext_out
);

__PATH_INTERNAL_END__
