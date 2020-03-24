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
 * An implementation of render/engine.h
 */

#include "./engine.h"

// Framework modules.
#include "./library/builtin.h"


__RENDER_BEGIN__

Engine::Engine(
	env::IEnvironment* env,
	const std::vector<std::string>& dirs,
	bool use_app_dirs,
	bool debug,
	bool auto_escape,
	std::vector<std::shared_ptr<ILoader>>& loaders,
	std::vector<std::shared_ptr<lib::ILibrary>>& libs,
	core::ILogger* logger
)
{
	this->_logger = logger;
	this->_env = env;
	for (const auto& dir : dirs)
	{
		if (core::str::ends_with(core::str::rtrim(dir, "/"), this->APP_DIRNAME))
		{
			this->_dirs.push_back(dir);
		}
		else
		{
			this->_dirs.push_back(core::path::join(dir, this->APP_DIRNAME));
		}
	}

	this->_use_app_dirs = use_app_dirs;
	this->_debug = debug;
	this->_auto_escape = auto_escape;
	if (loaders.empty())
	{
		this->_loaders.push_back(std::make_unique<DefaultLoader>());
	}
	else
	{
		this->_loaders = std::move(loaders);
	}

	this->_libraries = libs;
	if (!this->_debug)
	{
		for (const auto& loader : this->_loaders)
		{
			auto templates = loader->cache_templates(dirs, this);
			this->_cached_templates.insert(templates.begin(), templates.end());
		}
	}
}

std::shared_ptr<ITemplate> Engine::find_template(
	const std::string& name,
	const std::vector<std::string>& dirs
)
{
	std::vector<std::string> tried{};
	if (this->_debug)
	{
		// Load template from disk.
		for (const auto& loader : this->_loaders)
		{
			if (!loader)
			{
				continue;
			}

			try
			{

				return loader->get_template(name, dirs, this);
			}
			catch (const TemplateDoesNotExist& exc)
			{
				auto exc_tried = exc.tried();
				tried.insert(tried.begin(), exc_tried.begin(), exc_tried.end());
			}
		}
	}
	else
	{
		// Searches for template in memory.
		for (const auto& dir : dirs)
		{
			std::string template_name = core::path::join(dir, name);
			auto result = this->_cached_templates.find(template_name);
			if (result != this->_cached_templates.end())
			{
				return result->second;
			}
		}
	}

	throw TemplateDoesNotExist(name, tried, this->_env, _ERROR_DETAILS_);
}

std::shared_ptr<ITemplate> Engine::from_string(const std::string& template_code)
{
	return std::make_shared<Template>(template_code, this);
}

std::shared_ptr<ITemplate> Engine::get_template(const std::string& template_name)
{
	return this->find_template(template_name, this->_dirs);
}

env::IEnvironment* Engine::environment()
{
	return this->_env;
}

lib::Filters& Engine::get_filters()
{
	return this->_filters;
}

lib::Tags& Engine::get_tags()
{
	return this->_tags;
}

void Engine::load_libraries()
{
	if (!this->_libraries.empty())
	{
		if (dynamic_cast<lib::BuiltinLibrary*>(this->_libraries[0].get()) == nullptr)
		{
			throw core::ImproperlyConfigured("render::lib::BuiltinLibrary must be first");
		}

		this->_include_lib(this->_libraries[0].get(), true);
		for (size_t i = 1; i < this->_libraries.size(); i++)
		{
			if (dynamic_cast<lib::BuiltinLibrary*>(this->_libraries[i].get()) != nullptr)
			{
				throw core::ImproperlyConfigured("render::lib::BuiltinLibrary must appear only once");
			}

			this->_include_lib(this->_libraries[i].get(), false);
		}
	}
}

void Engine::_include_lib(lib::ILibrary* lib, bool is_builtin)
{
	auto filters_ptr = lib->get_filters();
	if (filters_ptr)
	{
		auto filters = *filters_ptr;
		for (const auto& filter : filters)
		{
			if (!is_builtin && this->_filters.contains(filter.first))
			{
				std::string lib_name = lib->name();
				this->_logger->warning(
					"'" + lib->name() + "' contains filter with the same name as the default." +
					"Filter: '" + filter.first + "', library: '" + lib_name + "'"
				);
				this->_filters.set(lib_name + "." + filter.first, filter.second);
			}
			else
			{
				this->_filters.set(filter.first, filter.second);
			}
		}
	}

	auto tags_ptr = lib->get_tags();
	if (tags_ptr)
	{
		auto tags = *tags_ptr;
		for (const auto& tag : tags)
		{
			if (!is_builtin && this->_tags.contains(tag.first))
			{
				std::string lib_name = lib->name();
				this->_logger->warning(
					"'" + lib->name() + "' contains tag with the same name as the default." +
					"Tag name: '" + tag.first + "', library: '" + lib_name + "'"
				);
				this->_tags.set(lib_name + "." + tag.first, tag.second);
			}
			else
			{
				this->_tags.set(tag.first, tag.second);
			}
		}
	}
}

__RENDER_END__
