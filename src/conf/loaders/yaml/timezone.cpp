/**
 * conf/loaders/yaml/timezone.cpp
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include "./timezone.h"

// Base libraries.
#include <xalwart.base/string_utils.h>


__CONF_BEGIN__

YAMLTimezoneComponent::YAMLTimezoneComponent(std::shared_ptr<dt::Timezone>& timezone) : timezone(timezone)
{
	this->register_component("name", std::make_unique<config::YAMLScalarComponent>(this->timezone_name));
	auto offset_component = std::make_unique<config::YAMLMapComponent>();
	offset_component->register_component(
		"days", std::make_unique<config::YAMLScalarComponent>(this->timezone_offset.days)
	);
	offset_component->register_component(
		"seconds", std::make_unique<config::YAMLScalarComponent>(this->timezone_offset.seconds)
	);
	offset_component->register_component(
		"microseconds", std::make_unique<config::YAMLScalarComponent>(this->timezone_offset.microseconds)
	);
	offset_component->register_component(
		"milliseconds", std::make_unique<config::YAMLScalarComponent>(this->timezone_offset.milliseconds)
	);
	offset_component->register_component(
		"minutes", std::make_unique<config::YAMLScalarComponent>(this->timezone_offset.minutes)
	);
	offset_component->register_component(
		"hours", std::make_unique<config::YAMLScalarComponent>(this->timezone_offset.hours)
	);
	offset_component->register_component(
		"weeks", std::make_unique<config::YAMLScalarComponent>(this->timezone_offset.weeks)
	);
	this->register_component("offset", std::move(offset_component));
}

void YAMLTimezoneComponent::initialize(const YAML::Node& node) const
{
	this->components.front().second->initialize(node[this->components.front().first]);
	if (!this->timezone_name.empty())
	{
		if (str::to_upper(this->timezone_name) == "UTC")
		{
			this->timezone = std::make_shared<dt::Timezone>(dt::Timezone::UTC);
		}
		else
		{
			this->components.back().second->initialize(node[this->components.back().first]);
			this->timezone = std::make_shared<dt::Timezone>(this->timezone_offset.to_timedelta(), this->timezone_name);
		}
	}
}

__CONF_END__
