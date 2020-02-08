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
 * Purpose:
 * TODO: write docs for render/backends/wasp.h
 */

#pragma once

// Module definitions.
#include "../_def_.h"

// Wasp libraries.
#include "./base.h"
#include "../engine.h"
#include "../../collections/dict.h"
#include "../../core/logger.h"
#include "../../core/exceptions.h"


__BACKENDS_BEGIN__

class WaspBackend : public BaseBackend
{
protected:
	Engine* _engine;

public:
	struct Options final
	{
		bool debug = false;
		core::ILogger* logger = core::Logger::get_instance({});
		std::vector<ILoader*> loaders;
		collections::Dict<std::string, Filter> filters;
		bool auto_escape = true;

		/*
		Options(
			core::ILogger* logger = nullptr,
			bool auto_escape = true,
			bool debug = false,
			const std::vector<ILoader*>& loaders = {},
			const collections::Dict<std::string, Filter>& filters = {}
		)
		{
			if (!logger)
			{
				logger = core::Logger::get_instance({});
			}

			this->logger = logger,
			this->auto_escape = auto_escape;
			this->debug = debug;
			this->loaders = loaders;
			this->filters = filters;
		}
		*/

		~Options()
		{
			for (auto loader : this->loaders)
			{
				delete loader;
			}
		}
	};

	WaspBackend(
		const std::vector<std::string>& dirs,
		bool use_app_dirs,
		const Options& opts
	);
	~WaspBackend() override;

	ITemplate* from_string(const std::string& template_code) override;
	ITemplate* get_template(const std::string& template_path) override;
};

__BACKENDS_END__
