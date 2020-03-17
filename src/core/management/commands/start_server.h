/*
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
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
 * core/managements/commands/runserver.h
 *
 * Purpose: runs server from command line.
 */

#pragma once

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "./app_command.h"
#include "../../../apps/interfaces.h"
#include "../../../urls/url.h"
#include "../../../urls/resolver.h"
#include "../../../http/request.h"
#include "../../../core/net/http_server.h"
#include "../../../core/parsers/url_parser.h"
#include "../../../core/datetime/datetime.h"
#include "../../../core/exceptions.h"
#include "../../../core/flags/flags.h"
#include "../../../core/string/str.h"
#include "../../../core/string/format.h"
#include "../../../core/regex.h"


__CORE_COMMANDS_BEGIN__

class StartServerCommand final: public AppCommand
{
private:
	flags::StringFlag* _host_port_flag;
	flags::StringFlag* _host_flag;
	flags::LongIntFlag* _port_flag;
	flags::LongIntFlag* _threads_flag;
	flags::BoolFlag* _use_ipv6_flag;

	rgx::Regex* _ipv4_ipv6_port_regex;
	rgx::Regex* _ipv4_regex;
	rgx::Regex* _ipv6_regex;
	rgx::Regex* _port_regex;

	const std::string DEFAULT_IPV4_HOST = "127.0.0.1";
	const std::string DEFAULT_IPV6_HOST = "[::1]";
	const uint16_t DEFAULT_PORT = 8000;
	const size_t DEFAULT_THREADS = 16;

protected:
	void add_flags() final;
	void handle() final;
	std::function<void(http::HttpRequest*, const core::net::internal::socket_t&)> get_handler();
	bool static_is_allowed(const std::string& static_url);
	void build_static_patterns(std::vector<urls::UrlPattern>& patterns);
	void build_app_patterns(std::vector<urls::UrlPattern>& patterns);
	void setup_server_ctx(core::net::internal::HttpServer::context& ctx);

	static http::HttpResponseBase* process_request_middleware(
		http::HttpRequest* request, conf::Settings* settings
	);
	static http::HttpResponseBase* process_response_middleware(
		http::HttpRequest* request,
		http::HttpResponseBase* response,
		conf::Settings* settings
	);
	static http::HttpResponseBase* process_urlpatterns(
		http::HttpRequest* request,
		std::vector<urls::UrlPattern>& urlpatterns,
		conf::Settings* settings
	);
	static void send_response(
		http::HttpRequest* request,
		http::HttpResponseBase* response,
		const core::net::internal::socket_t& client,
		conf::Settings* settings
	);
	static void log_request(
		const std::string& info,
		unsigned short status_code,
		conf::Settings* settings
	);

public:
	explicit StartServerCommand(apps::IAppConfig* config, conf::Settings* settings);
	~StartServerCommand() final;
};

__CORE_COMMANDS_END__
