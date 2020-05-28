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
 * core/yaml/_def_.h
 *
 * Purpose: core/yaml module's definitions.
 */

#pragma once

#include "../_def_.h"


// core::yaml
#define __YAML_BEGIN__ __CORE_BEGIN__ namespace yaml {
#define __YAML_END__ } __CORE_END__

// core::yaml::internal
#define __YAML_INTERNAL_BEGIN__ __YAML_BEGIN__ namespace internal {
#define __YAML_INTERNAL_END__ } __YAML_END__
