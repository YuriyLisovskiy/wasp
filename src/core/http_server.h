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
 * http_server.h
 * Purpose: http server based on tcp/ip socket.
 */

#pragma once

// C++ libraries.
#include <iostream>
#include <cstdint>
#include <string>
#include <functional>
#include <vector>
#include <fstream>
#include <thread>
#include <poll.h>

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "../utility/logger.h"
#include "../conf/defaults.h"
#include "../conf/settings.h"
#include "../http/response.h"
#include "../http/request.h"
#include "../http/headers.h"
#include "./parsers/request_parser.h"
#include "../core/sockets/server_socket.h"
#include "../core/exceptions.h"
#include "../core/thread_pool.h"
#include "../core/files/uploaded_file.h"
#include "../core/datetime/time.h"
#include "../utility/string/str.h"
#include "../utility/string/format.h"


__CORE_INTERNAL_BEGIN__

#define MAX_BUFF_SIZE 8192 * 8 - 1  // 65535 bytes.

// 4K per header value.
// 35 main headers and 3 additional.
// 403 bytes is a length of all headers names.
#define MAX_HEADERS_SIZE 4096 * 38 + 403    // 156051 bytes.

typedef std::function<void(http::HttpRequest*, const socket_t&)> http_handler;


class HttpServer
{
private:
	const char* _host;
	uint16_t _port;
	const char* _schema;
	bool _use_ipv6;
	bool _verbose;
	size_t _max_body_size;
	ServerSocket _server_socket;
	http_handler _http_handler;
	bool _finished;
	utility::ILogger* _logger;
	std::string _media_root;
	size_t _threads_count;
	core::internal::ThreadPool* _thread_pool;

	enum read_result_enum
	{
		rr_continue, rr_break, rr_none
	};

	int _init();
	int _create();
	int _bind();
	int _listen();
	void _clean_up(const socket_t& client);
	http::HttpRequest* _handle_request(const socket_t& client);
	std::string _read_body(const socket_t& client, const std::string& body_beginning, size_t body_length);
	static std::string _read_headers(const socket_t& client, std::string& body_beginning);
	void _start_listener();
	void _serve_connection(const socket_t& client);
	void _thread_func(const socket_t& client);

	static read_result_enum _handle_error(char* buffer, int line, const char* function, const char* file);
	static void _send(const char* data, const socket_t& client);
	static bool _set_socket_blocking(int _sock, bool blocking);
	static void _write(const char* data, size_t bytes_to_write, const socket_t& client);
	static void _wsa_clean_up();

public:
	struct context
	{
		std::string host;
		uint16_t port = 0;
		http_handler handler = nullptr;
		utility::ILogger* logger;
		size_t max_body_size = 0;
		std::string media_root;
		size_t threads_count = 0;
		bool use_ipv6 = false;
		bool verbose = false;
	};

	explicit HttpServer(HttpServer::context& ctx);
	~HttpServer();
	void finish();
	void listen_and_serve();
	static void send(http::HttpResponseBase* response, const socket_t& client);
	static void send(http::StreamingHttpResponse* response, const socket_t& client);

private:
	static void _check_context(HttpServer::context& ctx);
};

__CORE_INTERNAL_END__
