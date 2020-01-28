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
#include "./utility.h"
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
		collections::MultiValueDict<_Key, _Val> _multi_dict;

	public:
		explicit Parameters() = default;

		explicit Parameters(
			collections::Dict<_Key, _Val> dict,
			collections::MultiValueDict<_Key, _Val> multi_dict
		)
		{
			this->_dict = dict;
			this->_multi_dict = multi_dict;
		}

		_Val get(_Key key, _Val _default = _Val{})
		{
			return this->_dict.get(key, _default);
		}

		std::vector<_Val> get_list(_Key key, std::vector<_Val> _default = std::vector<_Val>{})
		{
			return this->_multi_dict.get(key, _default);
		}

		bool contains(_Key key)
		{
			return this->_dict.contains(key);
		}

		bool contains_list(_Key key)
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
	bool is_secure(conf::Settings* settings);
	std::string scheme(conf::Settings* settings);

	/// Return the HTTP host using the environment or request headers.
	std::string get_host(
		bool use_x_forwarded_host, bool debug, std::vector<std::string> allowed_hosts
	);

protected:
	/// Return the HTTP host using the environment or request headers. Skip
	/// allowed hosts protection, so may return an insecure host.
	std::string get_raw_host(bool use_x_forwarded_host);

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
