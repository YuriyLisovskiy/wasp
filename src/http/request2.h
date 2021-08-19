/**
 * http/request.h
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 *
 * Purpose: TODO
 */

#pragma once

// C++ libraries.
#include <string>
#include <vector>
#include <memory>

// Base libraries.
#include <xalwart.base/collections/multi_dictionary.h>
#include <xalwart.base/exceptions.h>
#include <xalwart.base/io.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "./url.h"
#include "./cookie.h"
#include "./errors.h"
#include "./multipart.h"
#include "./internal/cookie_parser.h"
#include "./internal/utilities.h"
#include "../conf/_def_.h"


__HTTP_BEGIN__

// TESTME: Request2
// TODO: docs for 'Request2'
class Request2
{
public:
	// Specifies the HTTP method (GET, POST, PUT, etc.).
	std::string method;

	// Specifies the URI being requested.
	//
	// The URL is parsed from the URI supplied on the Request-Line.
	// For most requests, fields other than 'path' and 'raw_query' will
	// be empty. (See RFC 7230, Section 5.3)
	URL url;

	// The protocol version for incoming server requests.
	std::string proto;  // "HTTP/1.0"
	int proto_major;    // 1
	int proto_minor;    // 0

	// Contains the request header fields either received
	// by the server.
	//
	// If a server received a request with header lines,
	//
	//	Host: example.com
	//	accept-encoding: gzip, deflate
	//	Accept-Language: en-us
	//	fOO: Bar
	//	foo: two
	//
	// then
	//
	//	header = Header(std::map<std::string, std::vector<std::string>>{
	//	    {"Accept-Encoding", {"gzip, deflate"}},
	//	    {"Accept-Language", {"en-us"}},
	//	    {"Foo", {"Bar", "two"}},
	//	});
	//
	// For incoming requests, the Host header is promoted to the
	// 'Request::host' field and removed from the 'header' map.
	//
	// HTTP defines that header names are case-insensitive. The
	// request parser implements this by making the first character
	// and any characters following a hyphen uppercase and the
	// rest lowercase.
	collections::MultiDictionary<std::string, std::string> header;

	// Body is the request's body.
	io::IReader* body;

	// 'content_length' records the length of the associated content.
	// The value -1 indicates that the length is unknown.
	// Values >= 0 indicate that the given number of bytes were
	// written to body.
	long long int content_length;

	// 'transfer_encoding' lists the transfer encodings from outermost to
	// innermost. An empty list denotes the "identity" encoding.
	// 'transfer_encoding' can usually be ignored; chunked encoding is
	// automatically added and removed as necessary when sending and
	// receiving requests.
	std::vector<std::string> transfer_encoding;

	// 'host' specifies the host on which the
	// URL is sought. For HTTP/1 (per RFC 7230, section 5.4), this
	// is either the value of the "Host" header or the host name
	// given in the URL itself. For HTTP/2, it is the value of the
	// ":authority" pseudo-header field.
	// It may be of the form "host:port". For international domain
	// names, 'host' may be in Punycode or Unicode form.
	std::string host;

	// 'form' contains the parsed form data, including both the URL
	// field's query parameters and the PATCH, POST, or PUT form data.
	// This field is only available after 'parse_form' is called.
	std::unique_ptr<Query> form;

	// 'post_form' contains the parsed form data from PATCH, POST
	// or PUT body parameters.
	//
	// This field is only available after 'parse_form' is called.
	std::unique_ptr<Query> post_form;

	// 'multipart_form' is the parsed multipart form, including file uploads.
	// This field is only available after 'parse_multipart_form' is called.
	std::unique_ptr<multipart::Form> multipart_form;

protected:
	[[nodiscard]]
	std::pair<std::unique_ptr<multipart::Reader>, ParseFormError> multipart_reader(bool allow_mixed) const;

public:
	inline explicit Request2(
		std::string method, URL url, std::string proto,
		int proto_major, int proto_minor, collections::MultiDictionary<std::string, std::string> header,
		io::IReader* body, long long int content_length
	) : method(std::move(method)), url(std::move(url)), proto(std::move(proto)),
		proto_major(proto_major), proto_minor(proto_minor), header(std::move(header)),
		body(body), content_length(content_length)
	{
		this->host = this->url.host;
	}

	// 'proto_at_least' reports whether the HTTP protocol used
	// in the request is at least major.minor.
	[[nodiscard]]
	inline bool proto_at_least(int major, int minor) const
	{
		return this->proto_major > major || this->proto_major == major && this->proto_minor >= minor;
	}

	// 'user_agent' returns the client's User-Agent, if sent in the request.
	[[nodiscard]]
	inline std::string user_agent() const
	{
		return this->header.get("User-Agent", "");
	}

	// 'cookies' parses and returns the HTTP cookies sent with the request.
	[[nodiscard]]
	inline std::vector<Cookie> Cookies() const
	{
		return internal::read_cookies(this->header.getlist("Cookie", {}), "");
	}

	// 'cookie' returns the named cookie provided in the request or
	// 'nullptr' if not found.
	// If multiple cookies match the given name, only one cookie will
	// be returned.
	[[nodiscard]]
	inline std::unique_ptr<Cookie> cookie(const std::string& name) const
	{
		auto cookies = internal::read_cookies(this->header.getlist("Cookie", {}), name);
		if (cookies.empty())
		{
			return nullptr;
		}

		return std::make_unique<Cookie>(cookies[0]);
	}

	// 'referer' returns the referring URL, if sent in the request.
	//
	// 'referer' is misspelled as in the request itself, a mistake from the
	// earliest days of HTTP.  This value can also be fetched from the
	// 'header' dictionary as header.get("Referer"); the benefit of making it available
	// as a method is that the compiler can diagnose programs that use the
	// alternate (correct English) spelling req.referrer() but cannot
	// diagnose programs that use header.get("Referrer").
	[[nodiscard]]
	inline std::string referer() const
	{
		return this->header.get("Referer", "");
	}

	// 'parse_form' populates 'form' and 'post_form'.
	//
	// For all requests, 'parse_form' parses the raw query from the URL and updates
	// 'form'.
	//
	// For POST, PUT, and PATCH requests, it also reads the request body, parses it
	// as a form and puts the results into both 'post_form' and 'form'. Request body
	// parameters take precedence over URL query string values in 'form'.
	//
	// For other HTTP methods, or when the Content-Type is not
	// application/x-www-form-urlencoded, the request 'body' is not read, and
	// 'post_form' is initialized to a non-nil, empty value.
	//
	// 'parse_multipart_form' calls 'parse_form' automatically.
	// 'parse_form' is idempotent.
	ParseFormError parse_form();

	// ParseMultipartForm parses a request body as multipart/form-data.
	// The whole request body is parsed and up to a total of maxMemory bytes of
	// its file parts are stored in memory, with the remainder stored on
	// disk in temporary files.
	// ParseMultipartForm calls ParseForm if necessary.
	// After one call to ParseMultipartForm, subsequent calls have no effect.
	ParseFormError parse_multipart_form(long long int max_memory);
};

enum class ParseMediaTypeError
{
	// TODO:
	Nil
};

// TESTME: parse_media_type
// TODO: docs for 'parse_media_type'
extern std::tuple<std::string, std::map<std::string, std::string>, ParseMediaTypeError> parse_media_type(
	const std::string& content_type
);

// TESTME: valid_method
//
//    Method         = "OPTIONS"                ; Section 9.2
//                   | "GET"                    ; Section 9.3
//                   | "HEAD"                   ; Section 9.4
//                   | "POST"                   ; Section 9.5
//                   | "PUT"                    ; Section 9.6
//                   | "DELETE"                 ; Section 9.7
//                   | "TRACE"                  ; Section 9.8
//                   | "CONNECT"                ; Section 9.9
//                   | extension-method
//  extension-method = token
//    token          = 1*<any CHAR except CTLs or separators>
inline bool valid_method(const std::string& method)
{
	return !method.empty() && !std::any_of(method.begin(),  method.end(), internal::util::is_not_token);
}

// TESTME: has_port
// Given a string of the form "host", "host:port", or "[ipv6::address]:port",
// return true if the string includes a port.
extern bool has_port(const std::string& host);

// TESTME: remove_empty_port
// 'remove_empty_port' strips the empty port in ":port" to ""
// as mandated by RFC 3986 Section 6.2.3.
extern std::string remove_empty_port(const std::string& host);

// TESTME: new_request
// TODO: docs for 'new_request'
extern std::tuple<std::unique_ptr<Request2>, std::unique_ptr<BaseException>> new_request(
	const std::string& method, const std::string& raw_url, const std::string& proto,
	int proto_major, int proto_minor, const collections::MultiDictionary<std::string, std::string>& header,
	io::IReader* body_reader, long long int content_length
);

// TESTME: parse_post_form
// TODO: docs for 'parse_post_form'
extern std::tuple<Query, ParseFormError> parse_post_form(Request2* request);

__HTTP_END__
