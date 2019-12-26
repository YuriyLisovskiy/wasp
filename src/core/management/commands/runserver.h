/*
 * Copyright (c) 2019 Yuriy Lisovskiy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * runserver.h
 * Purpose: runs server from command line.
 */

#pragma once

// C++ libraries.

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "./command.h"
#include "../../../http/request.h"
#include "../../../core/http_server.h"
#include "../../../utility/flags/flags.h"
#include "../../../utility/string/str.h"


__CORE_COMMANDS_BEGIN__

class RunserverCommand final: public Command
{
private:
	flags::StringFlag* _host_port_flag;
	flags::StringFlag* _host_flag;
	flags::LongIntFlag* _port_flag;
	flags::LongIntFlag* _threads_flag;

protected:
	void add_flags() final;
	void handle() final;
	std::function<void(http::HttpRequest*, const core::internal::socket_t&)> get_handler();

public:
	explicit RunserverCommand(conf::Settings* settings);
};

__CORE_COMMANDS_END__
