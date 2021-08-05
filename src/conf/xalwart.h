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
	version_t version;

	conf::Settings* settings;

	std::function<std::shared_ptr<net::abc::IServer>(
		log::ILogger*, const Kwargs&, std::shared_ptr<dt::Timezone>
	)> server_initializer;

protected:
	net::HandlerFunc make_handler();

	bool static_is_allowed(const std::string& static_url) const;

	void build_static_pattern(
		std::vector<std::shared_ptr<urls::IPattern>>& patterns,
		const std::string& root, const std::string& url, const std::string& name
	) const;

	void build_module_patterns(std::vector<std::shared_ptr<urls::IPattern>>& patterns);

	http::result_t process_request_middleware(std::shared_ptr<http::HttpRequest>& request);

	http::result_t process_response_middleware(
		std::shared_ptr<http::HttpRequest>& request, std::shared_ptr<http::IHttpResponse>& response
	);

	http::result_t process_urlpatterns(
		std::shared_ptr<http::HttpRequest>& request, std::vector<std::shared_ptr<urls::IPattern>>& urlpatterns
	);

	std::shared_ptr<http::HttpRequest> make_request(
		net::RequestContext* ctx, collections::Dictionary<std::string, std::string> env
	);

//	static std::shared_ptr<http::IHttpResponse> error_to_response(const Error* err);

	uint start_response(net::RequestContext* ctx, const http::result_t& result);

	void finish_response(net::RequestContext* ctx, http::IHttpResponse* response);

private:
	std::string _help_message;

	// List of commands to run from command line.
	std::map<std::string, std::shared_ptr<cmd::BaseCommand>> _commands;

	void _setup_commands();

	void _extend_settings_commands(
		const std::vector<std::shared_ptr<cmd::BaseCommand>>& from, const std::string& module_name
	);

public:
	explicit MainApplication(
		const std::string& version,
		conf::Settings* settings,
		std::function<std::shared_ptr<net::abc::IServer>(
			log::ILogger*, const Kwargs&, std::shared_ptr<dt::Timezone>
		)> server_initializer
	);

	void execute(int argc, char** argv);
};

__CONF_END__
