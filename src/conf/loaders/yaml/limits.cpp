/**
 * conf/loaders/yaml/limits.cpp
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include "./limits.h"


__CONF_BEGIN__

YAMLLimitsComponent::YAMLLimitsComponent(Limits& limits)
{
	this->register_component(
		"file_upload_max_memory_size",
		std::make_unique<config::YAMLScalarComponent>(limits.FILE_UPLOAD_MAX_MEMORY_SIZE)
	);
	this->register_component(
		"data_upload_max_memory_size",
		std::make_unique<config::YAMLScalarComponent>(limits.DATA_UPLOAD_MAX_MEMORY_SIZE)
	);
	this->register_component(
		"data_upload_max_number_fields",
		std::make_unique<config::YAMLScalarComponent>(limits.DATA_UPLOAD_MAX_NUMBER_FIELDS)
	);
	this->register_component(
		"max_header_length", std::make_unique<config::YAMLScalarComponent>(limits.MAX_HEADER_LENGTH)
	);
	this->register_component(
		"max_headers_count", std::make_unique<config::YAMLScalarComponent>(limits.MAX_HEADERS_COUNT)
	);
}

__CONF_END__
