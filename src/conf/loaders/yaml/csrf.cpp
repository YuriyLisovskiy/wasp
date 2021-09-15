/**
 * conf/loaders/yaml/csrf.cpp
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include "./csrf.h"


__CONF_BEGIN__

YAMLCSRFComponent::YAMLCSRFComponent(CSRF& csrf)
{
	auto cookie_component = std::make_unique<config::YAMLMapComponent>();
	cookie_component->register_component("name", std::make_unique<config::YAMLScalarComponent>(csrf.COOKIE.NAME));
	cookie_component->register_component("age", std::make_unique<config::YAMLScalarComponent>(csrf.COOKIE.AGE));
	cookie_component->register_component("domain", std::make_unique<config::YAMLScalarComponent>(csrf.COOKIE.DOMAIN));
	cookie_component->register_component("path", std::make_unique<config::YAMLScalarComponent>(csrf.COOKIE.PATH));
	cookie_component->register_component("secure", std::make_unique<config::YAMLScalarComponent>(csrf.COOKIE.SECURE));
	cookie_component->register_component(
		"http_only", std::make_unique<config::YAMLScalarComponent>(csrf.COOKIE.HTTP_ONLY)
	);
	cookie_component->register_component(
		"same_site", std::make_unique<config::YAMLScalarComponent>(csrf.COOKIE.SAME_SITE)
	);
	this->register_component("cookie", std::move(cookie_component));
	this->register_component("header_name", std::make_unique<config::YAMLScalarComponent>(csrf.HEADER_NAME));
	this->register_component("use_sessions", std::make_unique<config::YAMLScalarComponent>(csrf.USE_SESSIONS));
	this->register_component(
		"trusted_origins", std::make_unique<config::YAMLSequenceComponent>([&csrf](const YAML::Node& node)
		{
			auto trusted_origin = node.as<std::string>("");
			if (!trusted_origin.empty())
			{
				csrf.TRUSTED_ORIGINS.push_back(trusted_origin);
			}
		})
	);
}

__CONF_END__
