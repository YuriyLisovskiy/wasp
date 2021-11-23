/**
 * conf/loaders/yaml/static.cpp
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include "./static.h"

// Base libraries.
#include <xalwart.base/path.h>


__CONF_BEGIN__

YAMLStaticComponent::YAMLStaticComponent(Static& static_, const std::string& base_directory)
{
	this->register_component(
		"root", std::make_unique<config::YAMLScalarComponent>([&, base_directory](const YAML::Node& root)
		{
			auto string_root = root.as<std::string>(static_.ROOT);
			static_.ROOT = path::Path(string_root).is_absolute() ?
				string_root : path::join(base_directory, string_root);
		})
	);
	this->register_component(
		"url", std::make_unique<config::YAMLScalarComponent>([&](const YAML::Node& url)
		{
			static_.URL = url.as<std::string>(static_.URL);
		})
	);
}

__CONF_END__
