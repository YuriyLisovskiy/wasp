/**
 * apps/xalwart.h
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 *
 * Purpose: an implementation of main application.
 */

#pragma once

// Core libraries.
#include <xalwart.core/net/server.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "../conf/settings.h"


__APPS_BEGIN__

class MainApplication final
{
protected:
	conf::Settings* settings;

	std::function<std::shared_ptr<net::IServer>(
		core::ILogger*,
		collections::Dict<std::string, std::string>
	)> server_initializer;

protected:
	net::HandlerFunc make_handler();
	bool static_is_allowed(const std::string& static_url);
	void build_static_patterns(std::vector<std::shared_ptr<urls::UrlPattern>>& patterns);
	void build_app_patterns(std::vector<std::shared_ptr<urls::UrlPattern>>& patterns);

	core::Result<std::shared_ptr<http::IHttpResponse>> process_request_middleware(
		std::shared_ptr<http::HttpRequest>& request
	);
	core::Result<std::shared_ptr<http::IHttpResponse>> process_response_middleware(
		std::shared_ptr<http::HttpRequest>& request,
		std::shared_ptr<http::IHttpResponse>& response
	);
	core::Result<std::shared_ptr<http::IHttpResponse>> process_urlpatterns(
		std::shared_ptr<http::HttpRequest>& request,
		std::vector<std::shared_ptr<urls::UrlPattern>>& urlpatterns
	);

	std::shared_ptr<http::HttpRequest> make_request(
		net::RequestContext* ctx,
		collections::Dict<std::string, std::string> env
	);

	static std::shared_ptr<http::IHttpResponse> error_to_response(const core::Error* err);

	uint start_response(
		net::RequestContext* ctx,
		const core::Result<std::shared_ptr<http::IHttpResponse>>& result
	);

	void finish_response(net::RequestContext* ctx, http::IHttpResponse* response);

private:
	std::string _help_message;

	// List of commands to run from command line.
	std::map<std::string, std::shared_ptr<cmd::BaseCommand>> _commands;

	void _setup_commands();
	void _extend_settings_commands_or_error(
		const std::vector<std::shared_ptr<cmd::BaseCommand>>& from,
		const std::function<std::string(const std::string& cmd_name)>& err_fn
	);
	void _perform_checks();

public:
	explicit MainApplication(
		conf::Settings* settings,
		std::function<std::shared_ptr<net::IServer>(
			core::ILogger*,
			collections::Dict<std::string, std::string>
		)> server_initializer
	);
	void execute(int argc, char** argv);
};

__APPS_END__
