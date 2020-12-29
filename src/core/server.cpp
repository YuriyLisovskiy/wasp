/**
 * core/server.cpp
 *
 * Copyright (c) 2020 Yuriy Lisovskiy
 */

#include "./server.h"

// Core libraries.
#include <xalwart.core/datetime.h>

// Framework libraries.
#include "../core/parsers/query_parser.h"
#include "../core/parsers/multipart_parser.h"


__CORE_BEGIN__

DefaultServer::DefaultServer(
	const server::Context& ctx, HttpHandlerFunc handler
) : server::HTTPServer(ctx, this->_make_handler()), _http_handler(std::move(handler))
{
}

server::HandlerFunc DefaultServer::_make_handler()
{
	return [&](int sock, server::internal::request_parser* parser, core::Error* err)
	{
		if (err)
		{
			xw::core::Error fail;
			if ((fail = this->send(sock, _from_error(err)->serialize().c_str())))
			{
				this->ctx.logger->error(fail.msg);
			}
		}
		else
		{
			auto request = this->_process_request(parser);
			auto result = this->_http_handler(request.get(), sock);
			std::shared_ptr<http::IHttpResponse> response;
			if (result.catch_(core::HttpError))
			{
				response = _from_error(&result.err);
			}
			else if (!result.value)
			{
				// Response was not instantiated, so return 204 - No Content.
				response = std::make_shared<http::HttpResponse>(204);
			}
			else
			{
				auto error = result.value->err();
				if (error)
				{
					response = _from_error(&error);
				}
				else
				{
					response = result.value;
				}
			}

			this->_send_response(request.get(), response.get(), sock, this->ctx.logger.get());
		}
	};
}

std::shared_ptr<http::HttpRequest> DefaultServer::_process_request(server::internal::request_parser* parser)
{
	core::internal::query_parser qp;
	http::HttpRequest::Parameters<std::string, xw::string> get_params, post_params;
	http::HttpRequest::Parameters<std::string, UploadedFile> files_params;
	if (!std::strlen(parser->content.c_str()))
	{
		qp.parse(parser->query);
		if (parser->method == "GET")
		{
			get_params = http::HttpRequest::Parameters(qp.dict, qp.multi_dict);
		}
		else if (parser->method == "POST")
		{
			post_params = http::HttpRequest::Parameters(qp.dict, qp.multi_dict);
		}
	}
	else
	{
		core::internal::multipart_parser mp(this->ctx.media_root);
		switch (parser->content_type)
		{
			case server::internal::request_parser::content_type_enum::ct_application_x_www_form_url_encoded:
				qp.parse(parser->content);
				if (parser->method == "GET")
				{
					get_params = http::HttpRequest::Parameters(qp.dict, qp.multi_dict);
				}
				else if (parser->method == "POST")
				{
					post_params = http::HttpRequest::Parameters(qp.dict, qp.multi_dict);
				}
				break;
			case server::internal::request_parser::content_type_enum::ct_application_json:
				break;
			case server::internal::request_parser::content_type_enum::ct_multipart_form_data:
				mp.parse(parser->headers["Content-Type"], parser->content);
				post_params = http::HttpRequest::Parameters(
					mp.post_values, mp.multi_post_value
				);
				files_params = http::HttpRequest::Parameters(
					mp.file_values, mp.multi_file_value
				);
				break;
			case server::internal::request_parser::content_type_enum::ct_other:
				break;
			default:
				throw ParseError("Unknown content type", _ERROR_DETAILS_);
		}
	}

	return std::make_shared<http::HttpRequest>(
		parser->method,
		parser->path,
		parser->major_v,
		parser->minor_v,
		parser->query,
		parser->keep_alive,
		parser->content,
		parser->headers,
		get_params,
		post_params,
		files_params
	);
}

std::shared_ptr<http::IHttpResponse> DefaultServer::_from_error(core::Error* err)
{
	unsigned short code;
	switch (err->type)
	{
		case EntityTooLargeError:
			code = 413;
			break;
		case PermissionDenied:
			code = 403;
			break;
		case NotFound:
		case FileDoesNotExistError:
			code = 404;
			break;
		case InternalServerError:
			code = 500;
			break;
		case SuspiciousOperation:
		case DisallowedHost:
		case DisallowedRedirect:
			code = 400;
			break;
		case HttpError:
		default:
			code = 500;
			break;
	}

	return std::make_shared<http::HttpResponse>(code, err->msg);
}

core::Error DefaultServer::_send(http::IHttpResponse* response, const int& client)
{
	return this->send(client, response->serialize().c_str());
}

core::Error DefaultServer::_send(http::StreamingHttpResponse* response, const int& client)
{
	std::string chunk;
	while (!(chunk = response->get_chunk()).empty())
	{
		auto err = this->write(client, chunk.c_str(), chunk.size());
		if (err)
		{
			return err;
		}
	}

	response->close();
	return core::Error::none();
}

void DefaultServer::_send_response(
	http::HttpRequest* request, http::IHttpResponse* response, const int& client, ILogger* logger
)
{
	if (response->is_streaming())
	{
		auto* streaming_response = dynamic_cast<http::StreamingHttpResponse*>(response);
		this->_send(streaming_response, client);
	}
	else
	{
		this->_send(response, client);
	}

	_log_request(
		request->method() + " " +
		request->path() + " HTTP/" +
		request->version(),
		response->status(),
		logger
	);
}

void DefaultServer::_log_request(
	const std::string& info, unsigned short status_code, ILogger* logger
)
{
	if (logger)
	{
		core::Logger::Color color = core::Logger::Color::GREEN;
		if (status_code >= 400)
		{
			color = core::Logger::Color::YELLOW;
		}
		else if (status_code >= 500)
		{
			color = core::Logger::Color::RED;
		}

		logger->print(
			"[" + core::dt::Datetime::now().strftime("%d/%b/%Y %T") + "] \"" +
			info + "\" " + std::to_string(status_code),
			color
		);
	}
}

__CORE_END__
