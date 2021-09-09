/**
 * conf/application.cpp
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 */

#include "./application.h"

// C++ libraries.
#include <iostream>

// Framework libraries.
#include "../management/module.h"
#include "../urls/resolver.h"
#include "../urls/utilities.h"
#include "../middleware/exception.h"


__CONF_BEGIN__

void Application::_setup_commands()
{
	auto request_handler = this->build_server_handler();
	auto core_module = mgmt::CoreModuleConfig(this->settings, std::move(request_handler));
	this->_extend_settings_commands(core_module.get_commands(), core_module.get_name());
	for (auto& installed_module : this->settings->MODULES)
	{
		this->_extend_settings_commands(installed_module->get_commands(), installed_module->get_name());
	}
}

void Application::_extend_settings_commands(
	const std::vector<std::shared_ptr<cmd::BaseCommand>>& from, const std::string& module_name
)
{
	for (auto& command : from)
	{
		if (std::find_if(
			this->_commands.begin(), this->_commands.end(),
			[command](const std::pair<std::string, std::shared_ptr<cmd::BaseCommand>>& pair) -> bool {
				return command->name() == pair.first;
			}
		) != this->_commands.end())
		{
			this->settings->LOGGER->warning(
				"Module with name '" + module_name + "' overrides commands with '" + command->name() + "' command"
			);
		}

		this->_commands[command->name()] = command;
	}
}

Application::Application(const std::string& version, conf::Settings* settings) : version(version)
{
	InterruptException::initialize();
	require_non_null(settings, "'settings' is not instantiated", _ERROR_DETAILS_);

	this->settings = settings;
	this->settings->prepare();
	this->settings->perform_checks();

	// Check if static files can be served and create necessary urls.
	this->build_static_pattern(
		this->settings->URLPATTERNS, this->settings->STATIC_ROOT, this->settings->STATIC_URL, "static"
	);
	this->build_static_pattern(
		this->settings->URLPATTERNS, this->settings->MEDIA_ROOT, this->settings->MEDIA_URL, "media"
	);

	// Retrieve main module patterns and append them to result.
	this->build_module_patterns(this->settings->URLPATTERNS);

	// Initialize template engine's libraries.
	if (this->settings->TEMPLATE_ENGINE == nullptr)
	{
		throw NullPointerException("'settings->TEMPLATE_ENGINE' is nullptr", _ERROR_DETAILS_);
	}

	this->settings->TEMPLATE_ENGINE->load_libraries();

	this->_setup_commands();

	size_t max_len = 0;
	for (auto& command : this->_commands)
	{
		auto new_len = command.second->name().size();
		if (new_len > max_len)
		{
			max_len = new_len;
		}
	}

	for (auto& command : this->_commands)
	{
		std::string indent(max_len - command.second->name().size(), ' ');
		this->_help_message += "  " + command.second->name() + indent + "  " + command.second->help_message() + '\n';
	}

	if (!this->_help_message.empty())
	{
		this->_help_message = "Usage:\n  application [command]\n\n"
			"Available Commands:\n" + this->_help_message +
			"\nUse \"application [command] --help\" for more information about a command.";
	}
	else
	{
		this->_help_message = "Application has not commands.\n\n";
	}
}

void Application::execute(int argc, char** argv) const
{
	if (argc > 1)
	{
		try
		{
			if (this->_commands.contains(argv[1]))
			{
				this->_commands.at(argv[1])->run_from_argv(argc, argv);
			}
			else
			{
				std::cout << "Command \"" << argv[1] << "\" is not found.\n\n";
			}
		}
		catch (const BaseException& exc)
		{
			this->settings->LOGGER->error(exc.what(), exc.line(), exc.function(), exc.file());
		}
		catch (const std::exception& exc)
		{
			this->settings->LOGGER->error(exc.what());
		}
		catch (...)
		{
			this->settings->LOGGER->error("Unknown error", _ERROR_DETAILS_);
		}
	}
	else
	{
		std::cout << this->_help_message;
	}
}

Application::HandlerFunction Application::build_server_handler() const
{
	return [this](
		net::RequestContext* context, const std::map<std::string, std::string>& environment
	) -> net::StatusCode
	{
		auto request = this->build_request(context, environment);
		auto middleware_chain = this->build_middleware_chain();
		auto response = middleware_chain(request.get());
		return start_response(context, response);
	};
}

std::unique_ptr<http::abc::IHttpResponse> Application::error_response(
	http::Request* request, net::StatusCode status_code, const std::string& message
) const
{
	auto [status, status_is_found] = net::get_status_by_code(status_code);
	if (!status_is_found)
	{
		require_non_null(this->settings->LOGGER.get(), _ERROR_DETAILS_)->warning(
			"Unknown status code: " + std::to_string(status_code)
		);
	}

	bool is_json = require_non_null(request, _ERROR_DETAILS_)->is_json();
	std::string content;
	if (is_json)
	{
		content = this->settings->render_json_error_template(status, message);
	}

	content = this->settings->render_html_error_template(status, message);
	return std::make_unique<http::Response>(status_code, content, is_json ? http::mime::APPLICATION_JSON : "");
}

middleware::Function Application::build_controller_handler() const
{
	return [this](http::Request* request) -> std::unique_ptr<http::abc::IHttpResponse>
	{
		require_non_null(request, _ERROR_DETAILS_);
		auto apply = urls::resolve(request->url.path, this->settings->URLPATTERNS);
		if (apply)
		{
			return apply(request, this->settings);
		}

		return this->error_response(request, 404, "The requested resource was not found.");
	};
}

middleware::Function Application::build_middleware_chain() const
{
	middleware::Function chain = this->build_controller_handler();
	auto middleware_count = (long long)this->settings->MIDDLEWARE.size();
	for (long long i = middleware_count; i >= 0; i--)
	{
		chain = this->settings->MIDDLEWARE[i](chain);
	}

	return middleware::Exception(this->settings)(chain);
}

bool Application::static_is_allowed(const std::string& static_url) const
{
	auto url = http::parse_url(static_url);

	// Allow serving local static files if debug and
	// static url is local.
	return this->settings->DEBUG && url.hostname().empty();
}

void Application::build_static_pattern(
	std::vector<std::shared_ptr<urls::IPattern>>& patterns,
	const std::string& root, const std::string& url, const std::string& name
) const
{
	if (!root.empty() && this->static_is_allowed(url))
	{
		patterns.push_back(urls::make_static(url, root, name));
	}
}

void Application::build_module_patterns(std::vector<std::shared_ptr<urls::IPattern>>& patterns) const
{
	if (!this->settings->MODULES.empty())
	{
		auto root_module = this->settings->MODULES.front();
		if (!root_module)
		{
			throw NullPointerException("'root_module' is nullptr", _ERROR_DETAILS_);
		}

		auto modules_patterns = root_module->get_urlpatterns();
		patterns.insert(patterns.end(), modules_patterns.begin(), modules_patterns.end());
	}
}

std::shared_ptr<http::Request> Application::build_request(
	net::RequestContext* context, std::map<std::string, std::string> environment
) const
{
	require_non_null(context, "'context' is nullptr", _ERROR_DETAILS_);
	context->body->set_limit((ssize_t)context->content_size);

	return std::make_shared<http::Request>(
		*context,
		this->settings->FILE_UPLOAD_MAX_MEMORY_SIZE,
		this->settings->DATA_UPLOAD_MAX_NUMBER_FIELDS,
		65535, // TODO: settings->MAX_HEADER_LENGTH
		100,   // TODO: settings->MAX_HEADERS_COUNT
//		this->settings->MAX_HEADER_LENGTH,
//		this->settings->MAX_HEADERS_COUNT,
		std::move(environment)
	);
}

net::StatusCode Application::start_response(
	net::RequestContext* ctx, const std::unique_ptr<http::abc::IHttpResponse>& response
) const
{
	http::Response no_content(204);
	if (!response)
	{
		this->settings->LOGGER->warning("Response was not instantiated, returned 204.", _ERROR_DETAILS_);
	}

	return this->finish_response(ctx, response ? response.get() : &no_content);
}

net::StatusCode Application::finish_response(net::RequestContext* ctx, http::abc::IHttpResponse* response) const
{
	if (response->is_streaming())
	{
		auto* streaming_response = dynamic_cast<http::StreamingResponse*>(response);
		std::string chunk;
		while (!(chunk = streaming_response->get_chunk()).empty())
		{
			if (!ctx->write(chunk.c_str(), chunk.size()))
			{
				this->settings->LOGGER->trace("Unable to send chunk", _ERROR_DETAILS_);
			}
		}

		response->close();
	}
	else
	{
		auto data = response->serialize();
		if (!ctx->write(data.c_str(), data.size()))
		{
			this->settings->LOGGER->trace("Unable to send response", _ERROR_DETAILS_);
		}
	}

	return response->get_status();
}

__CONF_END__
