/**
 * management/commands/runserver.h
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 *
 * Purpose: runs server from command line.
 */

#pragma once

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "../../commands/app_command.h"
#include "../../core/server/interfaces.h"


__MANAGEMENT_COMMANDS_BEGIN__

class StartServerCommand final : public xw::cmd::AppCommand
{
private:
	std::string _host;
	uint16_t _port = DEFAULT_PORT;
	bool _use_ipv6 = false;
	size_t _threads_count = DEFAULT_THREADS;

	std::shared_ptr<core::flags::StringFlag> _addr_port_flag;
	std::shared_ptr<core::flags::StringFlag> _addr_flag;
	std::shared_ptr<core::flags::Uint16Flag> _port_flag;
	std::shared_ptr<core::flags::UnsignedLongFlag> _threads_flag;
	std::shared_ptr<core::flags::BoolFlag> _use_ipv6_flag;

	rgx::Regex _ipv4_ipv6_port_regex;
	rgx::Regex _ipv4_regex;
	rgx::Regex _ipv6_regex;

	const std::string DEFAULT_IPV4_HOST = "127.0.0.1";
	const std::string DEFAULT_IPV6_HOST = "[::1]";
	const uint16_t DEFAULT_PORT = 8000;
	const size_t DEFAULT_THREADS = 16;

protected:
	void add_flags() final;
	void handle() final;
	std::function<
		core::Result<std::shared_ptr<http::IHttpResponse>>(http::HttpRequest*, const int&)
	> make_handler();
	bool static_is_allowed(const std::string& static_url);
	void build_static_patterns(std::vector<std::shared_ptr<urls::UrlPattern>>& patterns);
	void build_app_patterns(std::vector<std::shared_ptr<urls::UrlPattern>>& patterns);
	void retrieve_args(
		std::string& host, uint16_t& port, bool& use_ipv6, size_t& threads_count
	);

	core::Result<std::shared_ptr<http::IHttpResponse>> process_request_middleware(
		http::HttpRequest* request, conf::Settings* settings
	);
	core::Result<std::shared_ptr<http::IHttpResponse>> process_response_middleware(
		http::HttpRequest* request,
		http::IHttpResponse* response,
		conf::Settings* settings
	);
	core::Result<std::shared_ptr<http::IHttpResponse>> process_urlpatterns(
		http::HttpRequest* request,
		std::vector<std::shared_ptr<urls::UrlPattern>>& urlpatterns,
		conf::Settings* settings
	);

public:
	explicit StartServerCommand(apps::IAppConfig* config, conf::Settings* settings);
	collections::Dict<std::string, std::string> get_kwargs() override;
};

__MANAGEMENT_COMMANDS_END__
