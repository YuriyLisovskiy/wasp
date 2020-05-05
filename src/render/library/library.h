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
 * render/library/library.h
 */

#pragma once

// Module definitions.
#include "./_def_.h"

// Framework modules.
#include "./base.h"
#include "../../conf/settings.h"


__LIB_BEGIN__

class Library : public ILibrary
{
protected:
	conf::Settings* settings;

public:
	explicit Library(conf::Settings* settings);
	std::shared_ptr<Filters> get_filters() override;
	std::shared_ptr<Tags> get_tags() override;
};

__LIB_END__
