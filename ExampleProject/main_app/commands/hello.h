/*
 * Copyright (c) 2020 Yura Lisovskiy
 */

#pragma once

#include <xalwart/core/management/app_command.h>


class HelloCommand : public xw::core::AppCommand
{
public:
	HelloCommand(xw::apps::IAppConfig* config, xw::conf::Settings* settings);
	void handle() override;
};
