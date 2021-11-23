/**
 * settings/components/yaml/timezone.h
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 *
 * Purpose: TODO
 */

#pragma once

// C++ libraries.
#include <memory>

// Base libraries.
#include <xalwart.base/datetime.h>
#include <xalwart.base/config/components/yaml/default.h>

// Module definitions.
#include "../../_def_.h"


__CONF_BEGIN__

// TESTME: YAMLTimezoneComponent
// TODO: docs for 'YAMLTimezoneComponent'
class YAMLTimezoneComponent : public config::YAMLMapComponent
{
public:
	explicit YAMLTimezoneComponent(std::shared_ptr<dt::Timezone>& timezone);

	void initialize(const YAML::Node& node) const override;

protected:
	struct TimezoneOffset
	{
		long days = 0;
		long seconds = 0;
		long microseconds = 0;
		long milliseconds = 0;
		long minutes = 0;
		long hours = 0;
		long weeks = 0;

		[[nodiscard]]
		inline dt::Timedelta to_timedelta() const
		{
			return dt::Timedelta(
				this->days, this->seconds, this->microseconds,
				this->milliseconds, this->minutes, this->hours, this->weeks
			);
		}
	};

	std::string timezone_name;
	TimezoneOffset timezone_offset{};
	std::shared_ptr<dt::Timezone>& timezone;
};

__CONF_END__
