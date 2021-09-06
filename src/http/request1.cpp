/**
 * http/request.cpp
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 */

#include "./request1.h"
//
//// Base libraries.
//#include <xalwart.base/net/meta.h>
//
//// Framework libraries.
//#include "./headers.h"
//#include "./utility.h"
//#include "./exceptions.h"
//#include "../conf/settings.h"
//
//
//__HTTP_BEGIN__
//
//std::string Request1::scheme(std::pair<std::string, std::string>* secure_proxy_ssl_header) const
//{
//	if (secure_proxy_ssl_header)
//	{
//		auto header_val = this->headers.get(
//			secure_proxy_ssl_header->first, ""
//		);
//		if (!header_val.empty())
//		{
//			return header_val == secure_proxy_ssl_header->second ? "https" : "http";
//		}
//	}
//
//	// Default scheme.
//	return "http";
//}
//
//std::pair<std::string, std::shared_ptr<BaseException>> Request1::get_host(
//	bool use_x_forwarded_host, bool use_x_forwarded_port, bool debug, std::vector<std::string> allowed_hosts
//)
//{
//	auto host = this->get_raw_host(use_x_forwarded_host, use_x_forwarded_port);
//	if (debug && allowed_hosts.empty())
//	{
//		allowed_hosts = {".localhost", "127.0.0.1", "::1"};
//	}
//
//	std::string domain, port;
//	split_domain_port(host, domain, port);
//	if (!domain.empty() && validate_host(domain, allowed_hosts))
//	{
//		return {host, nullptr};
//	}
//
//	auto msg = "Invalid HTTP_HOST header: ." + host + ".";
//	if (!domain.empty())
//	{
//		msg += " You may need to add " + domain + " to ALLOWED_HOSTS.";
//	}
//	else
//	{
//		msg += " The domain name provided is not valid according to RFC 1034/1035.";
//	}
//
//	return {"", std::make_shared<exc::DisallowedHost>(msg, _ERROR_DETAILS_)};
//}
//
//std::string Request1::get_raw_host(bool use_x_forwarded_host, bool use_x_forwarded_port) const
//{
//	std::string host;
//	if (use_x_forwarded_host && this->headers.contains(http::X_FORWARDED_HOST))
//	{
//		host = this->headers.get(http::X_FORWARDED_HOST);
//	}
//	else if (this->META.contains("HTTP_HOST"))
//	{
//		host = this->META.get("HTTP_HOST");
//	}
//	else
//	{
//		host = this->META.get(net::meta::SERVER_NAME);
//		auto port = this->get_port(use_x_forwarded_port);
//		if (port != (this->is_secure(this->settings->SECURE_PROXY_SSL_HEADER) ? "443" : "80"))
//		{
//			host = host + ":" + port;
//		}
//	}
//
//	return host;
//}
//
//std::string Request1::get_port(bool use_x_forwarded_port) const
//{
//	std::string port;
//	if (use_x_forwarded_port && this->headers.contains(http::X_FORWARDED_PORT))
//	{
//		port = this->headers.get(http::X_FORWARDED_PORT);
//	}
//	else
//	{
//		port = this->META.get(net::meta::SERVER_PORT);
//	}
//
//	return port;
//}
//
//__HTTP_END__
