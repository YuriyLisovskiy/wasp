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


__RENDER_BEGIN__

Engine::Engine(
	backends::BaseBackend* backend,
	const std::vector<std::string>& dirs,
	bool use_app_dirs,
	bool debug,
	const std::vector<ILoader*>& loaders,
	const collections::Dict<std::string, Filter>& custom_filters,
	core::ILogger* logger
)
{
	this->_backend = backend;
	this->_dirs = dirs;
	this->_use_app_dirs = use_app_dirs;
	this->_debug = debug;
	this->_use_default_loaders = loaders.empty();
	if (this->_use_default_loaders)
	{
		this->_loaders = {
			new Loader()
		};
	}
	else
	{
		this->_loaders = loaders;
	}

	for (auto filter = custom_filters.cbegin(); filter != custom_filters.cend(); filter++)
	{
		if (core::str::starts_with(filter->first, "std::"))
		{
			logger->warning(R"(An attempt to extend "std" namespace with ")" + filter->first + "\" filter");
			break;
		}

		if (core::str::starts_with(filter->first, "wasp::"))
		{
			logger->warning(R"(An attempt to extend "wasp" namespace with ")" + filter->first + "\" filter");
			break;
		}
	}

	this->_filters = DEFAULT_FILTERS;
	for (auto filter = custom_filters.cbegin(); filter != custom_filters.cend(); filter++)
	{
		if (this->_filters.contains(filter->first))
		{
			logger->warning("Unable to modify default filters with \"" + filter->first + "\" filter");
		}
		else
		{
			this->_filters.set(filter->first, filter->second);
		}
	}
}

Engine::~Engine()
{
	if (this->_use_default_loaders)
	{
		for (auto loader : this->_loaders)
		{
			delete loader;
		}
	}
}

ITemplate* Engine::find_template(
	const std::string& name,
	const std::vector<std::string>& dirs
)
{
	std::vector<std::string> tried{};
	for (auto loader : this->_loaders)
	{
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

	throw TemplateDoesNotExist(name, tried, this->_backend, _ERROR_DETAILS_);
}

ITemplate* Engine::from_string(const std::string& template_code)
{
	return new Template(template_code, this);
}

ITemplate* Engine::get_template(const std::string& template_name)
{
	return this->find_template(template_name, this->_dirs);
}

std::string Engine::render_to_string(
	const std::string& template_name, IContext* context
)
{
	auto t = this->get_template(template_name);
	auto result = t->render(context);
	delete t;
	return result;
}

backends::BaseBackend* Engine::backend()
{
	return this->_backend;
}

__RENDER_END__
