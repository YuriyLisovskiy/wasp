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
 * render/backends/wasp.h
 *
 * Purpose: default Wasp's backend.
 */

#pragma once

// C++ libraries.
#include <memory>

// Module definitions.
#include "../_def_.h"

// Wasp libraries.
#include "./base.h"
#include "../engine.h"
#include "../../collections/dict.h"
#include "../../core/logger.h"
#include "../../core/exceptions.h"
#include "../library/base.h"


__BACKENDS_BEGIN__

class WaspBackend : public BaseBackend
{
protected:
	std::shared_ptr<IEngine> _engine;

public:
	struct Options final
	{
		bool debug = false;
		core::ILogger* logger = core::Logger::get_instance({});
		std::vector<std::shared_ptr<ILoader>> loaders;
		std::vector<std::shared_ptr<render::lib::ILibrary>> libraries;
		bool auto_escape = true;
	};

	WaspBackend(
		const std::vector<std::string>& dirs,
		bool use_app_dirs,
		const std::vector<apps::IAppConfig*>& installed_apps,
		const std::shared_ptr<Options>& opts = nullptr
	);

	std::shared_ptr<ITemplate> from_string(const std::string& template_code) override;
	std::shared_ptr<ITemplate> get_template(const std::string& template_path) override;

protected:
	std::shared_ptr<Options> _opts;
};

__BACKENDS_END__
