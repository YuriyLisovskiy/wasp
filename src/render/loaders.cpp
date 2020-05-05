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
 * An implementation of render/loader.h
 */

#include "./loaders.h"

// C++ libraries.
#include <experimental/filesystem>

// Framework modules.
#include "./exceptions.h"
#include "./template.h"
#include "../core/path.h"


__RENDER_BEGIN__

std::shared_ptr<ITemplate> DefaultLoader::get_template(
	const std::string& template_name,
	const std::vector<std::string>& dirs,
	IEngine* engine
)
{
	for (const auto& dir : dirs)
	{
		auto file = core::File(
			core::path::join(dir, template_name)
		);
		file.open();
		if (file.is_open())
		{
			auto* _template = new Template(file.read_str(), (BaseEngine*)engine);
			file.close();
			return std::shared_ptr<Template>(_template);
		}
	}

	throw TemplateDoesNotExist(
		template_name, dirs, nullptr, _ERROR_DETAILS_
	);
}

std::map<std::string, std::shared_ptr<ITemplate>> DefaultLoader::cache_templates(
	const std::vector<std::string>& dirs,
	IEngine* engine
)
{
	std::map<std::string, std::shared_ptr<ITemplate>> cache;
	for (const auto& dir : dirs)
	{
		for (auto& entry : std::experimental::filesystem::directory_iterator(dir))
		{
			const auto& template_path = entry.path();
			auto file = core::File(
				core::path::join(dir, template_path)
			);
			file.open();
			if (file.is_open())
			{
				cache[template_path] = std::make_shared<Template>(
					file.read_str(), (BaseEngine*) engine
				);
				file.close();
			}
		}
	}

	return cache;
}

__RENDER_END__
