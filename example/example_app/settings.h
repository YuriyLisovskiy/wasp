/*
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 */

#pragma once

#include <xalwart/conf/settings.h>


class Settings final: public xw::conf::Settings
{
public:
	Settings();
	void register_apps() override;
	void register_middleware() override;
	void register_libraries() override;
};
