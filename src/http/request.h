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

/*
 * http request definition.
 * TODO: write docs
 */

#ifndef WASP_HTTP_REQUEST_H
#define WASP_HTTP_REQUEST_H

#include <string>
#include <vector>
#include <map>

#include "../globals.h"
#include "../collections/dict.h"
#include "../collections/multi_dict.h"
#include "../core/files/uploaded_file.h"


__WASP_BEGIN__

class HttpRequest
{
public:
	template <typename _Key, typename _Val>
	class Parameters
	{
	private:
		Dict<_Key, _Val> _dict;
		MultiValueDict<_Key, _Val> _multiDict;

	public:
		explicit Parameters() = default;

		explicit Parameters(Dict<_Key, _Val> dict, MultiValueDict<_Key, _Val> multiDict)
		{
			this->_dict = dict;
			this->_multiDict = multiDict;
		}

		_Val get(_Key key, _Val _default = _Val{})
		{
			return this->_dict.get(key, _default);
		}

		std::vector<_Val> getList(_Key key, std::vector<_Val> _default = std::vector<_Val>{})
		{
			return this->_multiDict.get(key, _default);
		}

		bool contains(_Key key)
		{
			return this->_dict.contains(key);
		}

		bool containsList(_Key key)
		{
			return this->_multiDict.contains(key);
		}

		size_t size()
		{
			return this->_dict.size();
		}

		bool empty()
		{
			return this->_dict.isEmpty();
		}

		std::vector<std::string> keys()
		{
			return this->_dict.keys();
		}
	};

	HttpRequest() : _majorVersion(0), _minorVersion(0), _keepAlive(false) {};
	explicit HttpRequest(
		std::string method, std::string path, size_t majorV, size_t minorV,
		std::string query, bool keepAlive, std::string content,
		const std::map<std::string, std::string>& headers,
		const HttpRequest::Parameters<std::string, std::string>& getParameters,
		const HttpRequest::Parameters<std::string, std::string>& postParameters,
		const HttpRequest::Parameters<std::string, UploadedFile>& filesParameters
	);

	Dict<std::string, std::string> headers;
	HttpRequest::Parameters<std::string, std::string> GET;
	HttpRequest::Parameters<std::string, std::string> POST;
	Dict<std::string, std::string> COOKIES;
	HttpRequest::Parameters<std::string, UploadedFile> FILES;

	std::string version();
	std::string path();
	std::string method();
	std::string body();

private:
	size_t _majorVersion;
	size_t _minorVersion;
	std::string _path;
	std::string _query;
	std::string _method;
	bool _keepAlive;
	std::string _body;
};

__WASP_END__


#endif // WASP_HTTP_REQUEST_H
