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
 * _def_.h
 * Purpose: render module's definitions.
 */

#pragma once

#include "../_def_.h"


// wasp::render
#define __RENDER_BEGIN__ __WASP_BEGIN__ namespace render {
#define __RENDER_END__ } __WASP_END__

// wasp::render::backends
#define __BACKENDS_BEGIN__ __RENDER_BEGIN__ namespace backends {
#define __BACKENDS_END__ } __RENDER_END__
