/**
 * conf/loaders/yaml_loader.cpp
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include "./yaml_loader.h"


__CONF_BEGIN__

/*void YAMLSettingsLoader::overwrite_template_engine(
	YAML::Node& template_engine, const YAML::Node& local_template_engine
)
{
	if (local_template_engine.IsNull())
	{
		template_engine = null_node();
	}
	else if (local_template_engine.IsMap())
	{
		if (!template_engine)
		{
			template_engine = map_node();
		}

		this->overwrite_sequence_or_remove_if_null(
			template_engine, local_template_engine, "directories"
		);
		this->overwrite_sequence_or_remove_if_null(
			template_engine, local_template_engine, "libraries"
		);
		this->overwrite_sequence_or_remove_if_null(
			template_engine, local_template_engine, "loaders"
		);
		this->overwrite_scalar_or_remove_if_null(
			template_engine, local_template_engine, "use_module_directories"
		);
		this->overwrite_scalar_or_remove_if_null(
			template_engine, local_template_engine, "auto_escape"
		);
	}
}*/

/*void YAMLSettingsLoader::init_template_engine_setting(Settings* settings, const YAML::Node& config)
{
	std::vector<std::string> dirs;
	auto directories = config["directories"];
	if (directories && directories.IsSequence())
	{
		for (auto it = directories.begin(); it != directories.end(); it++)
		{
			if (it->IsDefined() && it->IsScalar())
			{
				auto p = it->as<std::string>();
				dirs.push_back(
					path::is_absolute(p) ? p : path::join(settings->BASE_DIR.to_string(), p)
				);
			}
		}
	}

	std::vector<std::shared_ptr<render::abc::ILibrary>> libs{
		settings->build_library(render::DefaultLibrary::FULL_NAME)
	};
	auto libraries = config["libraries"];
	if (libraries && libraries.IsSequence() && libraries.size() > 0)
	{
		settings->register_libraries();
		for (auto it = libraries.begin(); it != libraries.end(); it++)
		{
			if (it->IsDefined() && it->IsScalar())
			{
				libs.push_back(settings->build_library(it->as<std::string>()));
			}
		}
	}

	std::vector<std::shared_ptr<render::abc::ILoader>> loaders_vec;
	auto loaders = config["loaders"];
	if (loaders && loaders.IsSequence() && loaders.size() > 0)
	{
		settings->register_loaders();
		for (auto it = loaders.begin(); it != loaders.end(); it++)
		{
			if (it->IsDefined() && it->IsScalar())
			{
				loaders_vec.push_back(settings->build_loader(it->as<std::string>()));
			}
		}
	}

	auto auto_escape = config["auto_escape"];
	settings->TEMPLATE_ENGINE = std::make_unique<render::DefaultEngine>(
		dirs,
		settings->DEBUG,
		auto_escape && !auto_escape.IsNull() && auto_escape.as<bool>(false),
		loaders_vec,
		libs,
		settings->LOGGER.get()
	);
}*/

__CONF_END__
