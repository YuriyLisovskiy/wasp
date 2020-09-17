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
 * render/engine.h
 *
 * Purpose:
 *  TODO: implement docs for render/engine.h
 */

#pragma once

// Module definitions.
#include <xalwart.render/_def_.h>

// Framework modules.
#include <xalwart.render/engine.h>
#include "./env/config.h"


__RENDER_BEGIN__

class Engine : public DefaultEngine
{
public:
	Engine(env::Config* cfg);
};

__RENDER_END__
