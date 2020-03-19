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
 * apps/wasp.h
 *
 * Purpose: an implementation of main Wasp application.
 */

#pragma once

// C++ libraries.
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <functional>

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "./config.h"
#include "../conf/settings.h"
#include "../core/path.h"
#include "../core/exceptions.h"
#include "../core/management/base.h"
#include "../core/management/app.h"


__APPS_BEGIN__

class WaspApplication final
{
private:
	conf::Settings* _settings;
	std::string _help_message;

	void _setup_commands();
	void _extend_settings_commands_or_error(
		const std::vector<std::shared_ptr<core::BaseCommand>>& from,
		const std::function<std::string(const std::string& cmd_name)>& err_fn
	);
	void _perform_checks();

public:
	explicit WaspApplication(conf::Settings* settings);
	void execute(int argc, char** argv);
};

__APPS_END__
