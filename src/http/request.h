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
 * request.h
 * Purpose: represents an http request.
 */

#pragma once

// C++ libraries.
#include <string>
#include <vector>
#include <map>

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "./headers.h"
#include "../core/string/str.h"
#include "../collections/dict.h"
#include "../collections/multi_dict.h"
#include "../core/files/uploaded_file.h"
#include "../core/exceptions.h"
#include "../conf/settings.h"


__HTTP_BEGIN__

class HttpRequest
{
public:
	template <typename _Key, typename _Val>
	class Parameters
	{
	private:
		collections::Dict<_Key, _Val> _dict;
		collections::MultiValueDict<_Key, _Val> _multiDict;

	public:
		explicit Parameters() = default;

		explicit Parameters(
			collections::Dict<_Key, _Val> dict,
			collections::MultiValueDict<_Key, _Val> multiDict
		)
		{
			this->_dict = dict;
			this->_multiDict = multiDict;
		}

		_Val get(_Key key, _Val _default = _Val{})
		{
			return this->_dict.get(key, _default);
		}

		std::vector<_Val> get_list(_Key key, std::vector<_Val> _default = std::vector<_Val>{})
		{
			return this->_multiDict.get(key, _default);
		}

		bool contains(_Key key)
		{
			return this->_dict.contains(key);
		}

		bool contains_list(_Key key)
		{
			return this->_multiDict.contains(key);
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
		std::string query, bool keep_alive, std::string content,
		const std::map<std::string, std::string>& headers,
		const HttpRequest::Parameters<std::string, std::string>& get_params,
		const HttpRequest::Parameters<std::string, std::string>& post_params,
		const HttpRequest::Parameters<std::string, core::UploadedFile>& files_params
	);

	collections::Dict<std::string, std::string> headers;
	HttpRequest::Parameters<std::string, std::string> GET;
	HttpRequest::Parameters<std::string, std::string> POST;
	collections::Dict<std::string, std::string> COOKIES;
	HttpRequest::Parameters<std::string, core::UploadedFile> FILES;

	std::string version();
	std::string path();
	std::string full_path(bool force_append_slash = false);
	std::string query();
	std::string method();
	bool keep_alive();
	std::string body();
	bool is_secure();
	std::string scheme();

	/// Return the HTTP host using the environment or request headers.
	std::string get_host(conf::Settings* settings);

protected:
	/// Return the HTTP host using the environment or request headers. Skip
	/// allowed hosts protection, so may return an insecure host.
	std::string get_raw_host(bool use_x_forwarded_host);

	/// Writes domain and port from a given host.
	///
	/// Returned domain is lower-cased. If the host is invalid,
	/// the domain will be empty.
	void split_domain_port(
		const std::string& host, std::string& domain, std::string& port
	);

	/// Validate the given host for this site.
	///
	/// Check that the host looks valid and matches a host or host pattern in the
	/// given list of `allowed_hosts`. Any pattern beginning with a period
	/// matches a domain and all its sub-domains (e.g. `.example.com` matches
	/// `example.com` and any sub-domain), `*` matches anything, and anything
	/// else must match exactly.
	///
	/// Note: This function assumes that the given host is lower-cased and has
	/// already had the port, if any, stripped off.
	///
	/// Return `true` for a valid host, `false` otherwise.
	bool validate_host(
		const std::string& domain, const std::vector<std::string>& allowed_hosts
	);

private:
	size_t _major_version;
	size_t _minor_version;
	std::string _path;
	std::string _query;
	std::string _method;
	bool _keep_alive;
	std::string _body;
};

__HTTP_END__
