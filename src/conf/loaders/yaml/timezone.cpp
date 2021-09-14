/**
 * conf/loaders/yaml/timezone.cpp
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include "./timezone.h"


__CONF_BEGIN__

/*void _initialize_name(std::string& value, const YAML::Node& name, const YAML::Node& local_name)
{
	auto component = local_name.IsScalar() || local_name.IsMap() ? local_name : name;
	value = component.as<std::string>("");
}

void initialize_timezone(Settings* settings, const YAML::Node& timezone, const YAML::Node& local_timezone)
{
	std::string timezone_name;
	_initialize_name(
		timezone_name,
		timezone.IsMap() ? timezone["name"] : null_node(),
		local_timezone.IsMap() ? local_timezone["name"] : null_node()
	);

	if (!timezone_name.empty())
	{
		if (str::to_upper(timezone_name) == "UTC")
		{
			settings->TIMEZONE = std::make_shared<dt::Timezone>(dt::Timezone::UTC);
		}
		else
		{
			YAML::Node offset_component;
			if (local_timezone.IsMap())
			{

			}


			auto timezone_offset = timezone["offset"];
			if (timezone_offset && timezone_offset.IsMap())
			{
				settings->TIMEZONE = std::make_shared<dt::Timezone>(
					dt::Timedelta(
						timezone_offset["days"].as<long>(0),
						timezone_offset["seconds"].as<long>(0),
						timezone_offset["microseconds"].as<long>(0),
						timezone_offset["milliseconds"].as<long>(0),
						timezone_offset["minutes"].as<long>(0),
						timezone_offset["hours"].as<long>(0),
						timezone_offset["weeks"].as<long>(0)
					),
					timezone_name
				);
			}
			else
			{
				settings->TIMEZONE = std::make_shared<dt::Timezone>(dt::Timedelta(), timezone_name);
			}
		}
	}
}*/

__CONF_END__
