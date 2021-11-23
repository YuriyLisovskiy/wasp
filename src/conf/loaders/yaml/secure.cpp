/**
 * conf/loaders/yaml/secure.cpp
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include "./secure.h"


__CONF_BEGIN__

YAMLSecureComponent::YAMLSecureComponent(Secure& secure)
{
	this->register_component(
		"browser_xss_filter", std::make_unique<config::YAMLScalarComponent>(secure.BROWSER_XSS_FILTER)
	);
	this->register_component(
		"content_type_no_sniff", std::make_unique<config::YAMLScalarComponent>(secure.CONTENT_TYPE_NO_SNIFF)
	);
	this->register_component(
		"hsts_include_subdomains", std::make_unique<config::YAMLScalarComponent>(secure.HSTS_INCLUDE_SUBDOMAINS)
	);
	this->register_component("hsts_preload", std::make_unique<config::YAMLScalarComponent>(secure.HSTS_PRELOAD));
	this->register_component("hsts_seconds", std::make_unique<config::YAMLScalarComponent>(secure.HSTS_SECONDS));
	this->register_component("referrer_policy", std::make_unique<config::YAMLScalarComponent>(secure.REFERRER_POLICY));
	this->register_component("ssl_host", std::make_unique<config::YAMLScalarComponent>(secure.SSL_HOST));
	this->register_component("ssl_redirect", std::make_unique<config::YAMLScalarComponent>(secure.SSL_REDIRECT));
	this->register_component(
		"redirect_exempt", std::make_unique<config::YAMLSequenceComponent>([&secure](const YAML::Node& node)
		{
			auto value = node.as<std::string>("");
			if (!value.empty())
			{
				secure.REDIRECT_EXEMPT.push_back(value);
			}
		})
	);
	auto proxy_ssl_header_component = std::make_unique<config::YAMLMapComponent>();
	proxy_ssl_header_component->register_component(
		"name", std::make_unique<config::YAMLScalarComponent>(secure.PROXY_SSL_HEADER->name)
	);
	proxy_ssl_header_component->register_component(
		"value", std::make_unique<config::YAMLScalarComponent>(secure.PROXY_SSL_HEADER->value)
	);
	this->register_component("proxy_ssl_header", std::move(proxy_ssl_header_component));
}

__CONF_END__
