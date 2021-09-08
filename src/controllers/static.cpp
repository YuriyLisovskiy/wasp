/**
 * controllers/static.cpp
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 */

#include "./static.h"

// Base libraries.
#include <xalwart.base/path.h>

// Framework libraries.
#include "../http/mime/media_type.h"


__CONTROLLERS_BEGIN__

bool was_modified_since(const std::string& header, size_t time, size_t size)
{
	bool result = false;
	if (header.empty())
	{
		result = true;
	}
	else
	{
		auto rgx = re::Regex("([^;]+)(; length=([0-9]+))?", std::regex_constants::icase);
		if (rgx.search(header))
		{
			auto header_time = http::parse_http_datetime(rgx.group(1));
			auto header_len = rgx.group(3);
			if (
				(!header_len.empty() && std::strtol(header_len.c_str(), nullptr, 10) != size) ||
				time > header_time
			)
			{
				result = true;
			}
		}
		else
		{
			result = true;
		}
	}

	return result;
}

std::unique_ptr<http::abc::IHttpResponse> StaticController::get(const std::string& p)
{
	if (!this->_kwargs.contains("document_root"))
	{
		throw KeyError("ctrl::StaticController requires \"document_root\"", _ERROR_DETAILS_);
	}

	auto full_path = path::join(this->_kwargs.get<std::string>("document_root", ""), p);
	if (!path::exists(full_path))
	{
		// TODO: add default http 404 error content!
		http::raise(404, this->_kwargs.get<std::string>("http_404", "404 Not Found"));
	}

	auto stat_info = file_stat(full_path);
	if (!was_modified_since(
		request->get_header(http::IF_MODIFIED_SINCE, ""), stat_info.st_mtime, stat_info.st_size
	))
	{
		return std::make_unique<http::resp::NotModified>("");
	}

	std::string content_type, encoding;
	http::mime::guess_content_type(full_path, content_type, encoding);
	if (content_type.empty())
	{
		content_type = "application/octet-stream";
	}

	auto response = std::make_unique<http::FileResponse>(full_path, false, 0, content_type);
	response->set_header(http::LAST_MODIFIED, http::http_date(stat_info.st_mtime));
	if (!encoding.empty())
	{
		response->set_header(http::CONTENT_ENCODING, encoding);
	}

	return response;
}

__CONTROLLERS_END__
