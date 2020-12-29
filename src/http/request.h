/**
 * http/request.h
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 *
 * Purpose: represents an http request.
 */

#pragma once

// Core libraries.
#include <xalwart.core/collections/multi_dict.h>
#include <xalwart.core/result.h>
#include <xalwart.core/string.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "../core/uploaded_file.h"


__HTTP_BEGIN__

class HttpRequest
{
public:
	template <typename Key, typename Val>
	class Parameters
	{
	private:
		collections::Dict<Key, Val> _dict;
		collections::MultiValueDict<Key, Val> _multi_dict;

	public:
		explicit Parameters() = default;

		explicit Parameters(
			collections::Dict<Key, Val> dict,
			collections::MultiValueDict<Key, Val> multi_dict
		)
		{
			this->_dict = dict;
			this->_multi_dict = multi_dict;
		}

		Val get(Key key, Val _default = Val{})
		{
			return this->_dict.get(key, _default);
		}

		std::vector<Val> get_list(Key key, std::vector<Val> _default = std::vector<Val>{})
		{
			return this->_multi_dict.get(key, _default);
		}

		bool contains(Key key)
		{
			return this->_dict.contains(key);
		}

		bool contains_list(Key key)
		{
			return this->_multi_dict.contains(key);
		}

		size_t size()
		{
			return this->_dict.size();
		}

		bool empty()
		{
			return this->_dict.is_empty();
		}

		std::vector<std::string> keys()
		{
			return this->_dict.keys();
		}
	};

	HttpRequest() : _major_version(0), _minor_version(0), _keep_alive(false) {};
	explicit HttpRequest(
		const std::string& method, std::string path, size_t major_v, size_t minor_v,
		std::string query, bool keep_alive, xw::string content,
		const std::map<std::string, std::string>& headers,
		const HttpRequest::Parameters<std::string, xw::string>& get_params,
		const HttpRequest::Parameters<std::string, xw::string>& post_params,
		const HttpRequest::Parameters<std::string, core::UploadedFile>& files_params
	);

	collections::Dict<std::string, std::string> headers;
	HttpRequest::Parameters<std::string, xw::string> GET;
	HttpRequest::Parameters<std::string, xw::string> POST;
	collections::Dict<std::string, std::string> COOKIES;
	HttpRequest::Parameters<std::string, core::UploadedFile> FILES;

	[[nodiscard]]
	std::string version() const;
	std::string path();
	std::string full_path(bool force_append_slash = false);
	std::string query();
	std::string method();

	[[nodiscard]]
	bool keep_alive() const;
	xw::string body();
	bool is_secure(std::pair<std::string, std::string>* secure_proxy_ssl_header) const;
	std::string scheme(std::pair<std::string, std::string>* secure_proxy_ssl_header) const;

	/// Return the HTTP host using the environment or request headers.
	core::Result<std::string> get_host(
		bool use_x_forwarded_host, bool debug, std::vector<std::string> allowed_hosts
	);

protected:
	/// Return the HTTP host using the environment or request headers. Skip
	/// allowed hosts protection, so may return an insecure host.
	[[nodiscard]]
	std::string get_raw_host(bool use_x_forwarded_host) const;

private:
	size_t _major_version;
	size_t _minor_version;
	std::string _path;
	std::string _query;
	std::string _method;
	bool _keep_alive;
	xw::string _body;
};

__HTTP_END__
