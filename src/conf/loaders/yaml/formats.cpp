/**
 * conf/loaders/yaml/formats.cpp
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include "./formats.h"


__CONF_BEGIN__

YAMLFormatsComponent::YAMLFormatsComponent(Formats& formats)
{
	this->register_component("date", std::make_unique<config::YAMLScalarComponent>(formats.DATE_FORMAT));
	this->register_component("datetime", std::make_unique<config::YAMLScalarComponent>(formats.DATETIME_FORMAT));
	this->register_component("time", std::make_unique<config::YAMLScalarComponent>(formats.TIME_FORMAT));
	this->register_component("year_month", std::make_unique<config::YAMLScalarComponent>(formats.YEAR_MONTH_FORMAT));
	this->register_component("month_day", std::make_unique<config::YAMLScalarComponent>(formats.MONTH_DAY_FORMAT));
	this->register_component("short_date", std::make_unique<config::YAMLScalarComponent>(formats.SHORT_DATE_FORMAT));
	this->register_component(
		"short_datetime", std::make_unique<config::YAMLScalarComponent>(formats.SHORT_DATETIME_FORMAT)
	);
}

__CONF_END__
