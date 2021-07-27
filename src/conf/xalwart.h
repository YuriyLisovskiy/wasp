/**
 * conf/xalwart.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * An implementation of main application.
 */

#pragma once

// Base libraries.
#include <xalwart.base/net/abc.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "../conf/settings.h"


__CONF_BEGIN__

class MainApplication
{
protected:
	conf::Settings* settings;
	std::function<std::shared_ptr<net::abc::IServer>(log::ILogger*, const Kwargs&)> server_initializer;

protected:
	net::HandlerFunc make_handler();
	bool static_is_allowed(const std::string& static_url);
	void build_static_patterns(std::vector<std::shared_ptr<urls::UrlPattern>>& patterns);
	void build_module_patterns(std::vector<std::shared_ptr<urls::UrlPattern>>& patterns);

	Result<std::shared_ptr<http::IHttpResponse>> process_request_middleware(
		std::shared_ptr<http::HttpRequest>& request
	);
	Result<std::shared_ptr<http::IHttpResponse>> process_response_middleware(
		std::shared_ptr<http::HttpRequest>& request,
		std::shared_ptr<http::IHttpResponse>& response
	);
	Result<std::shared_ptr<http::IHttpResponse>> process_urlpatterns(
		std::shared_ptr<http::HttpRequest>& request,
		std::vector<std::shared_ptr<urls::UrlPattern>>& urlpatterns
	);

	std::shared_ptr<http::HttpRequest> make_request(
		net::RequestContext* ctx,
		collections::Dict<std::string, std::string> env
	);

	static std::shared_ptr<http::IHttpResponse> error_to_response(const Error* err);

	uint start_response(
		net::RequestContext* ctx,
		const Result<std::shared_ptr<http::IHttpResponse>>& result
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

public:
	explicit MainApplication(
		conf::Settings* settings,
		std::function<std::shared_ptr<net::abc::IServer>(log::ILogger*, const Kwargs&)> server_initializer
	);
	void execute(int argc, char** argv);
};

__CONF_END__
