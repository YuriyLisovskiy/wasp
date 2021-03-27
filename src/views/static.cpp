/**
 * views/static.cpp
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 */

#include "./static.h"

// Core libraries.
#include <xalwart.core/path.h>

// Framework libraries.
#include "../http/headers.h"
#include "../http/utility.h"
#include "../core/mime_types.h"


__VIEWS_BEGIN__

StaticView::StaticView(conf::Settings* settings)
	: View({"get"}, settings)
{
}

void StaticView::set_kwargs(collections::Dict<std::string, std::string>* kwargs)
{
	this->_kwargs = kwargs;
}

core::Result<std::shared_ptr<http::IHttpResponse>> StaticView::get(http::HttpRequest* request, Args* args)
{
	if (!this->_kwargs)
	{
		throw core::ImproperlyConfigured(
			"views::StaticView requires kwargs", _ERROR_DETAILS_
		);
	}

	if (!this->_kwargs->contains("document_root"))
	{
		throw core::ImproperlyConfigured(
			"views::StaticView requires \"document_root\" argument", _ERROR_DETAILS_
		);
	}

	if (!args)
	{
		this->logger->warning(
			"unable to retrieve \"path\" argument from url while serving static file",
			_ERROR_DETAILS_
		);

		// TODO: add default http 404 error content!
		return this->response<http::HttpResponseNotFound>(this->_kwargs->get("http_404", "404 Not Found"));
	}

	auto full_path = path::join(this->_kwargs->get("document_root", ""), args->get_str("path", ""));
	if (!path::exists(full_path))
	{
		// TODO: add default http 404 error content!
		return this->response<http::HttpResponseNotFound>(this->_kwargs->get("http_404", "404 Not Found"));
	}

	auto stat_info = core::file_stat(full_path);
	if (!internal::was_modified_since(
		request->headers.get(http::IF_MODIFIED_SINCE, ""),
		stat_info.st_mtime, stat_info.st_size)
	)
	{
		return this->response<http::HttpResponseNotModified>("");
	}

	std::string content_type, encoding;
	core::mime::guess_content_type(full_path, content_type, encoding);
	if (content_type.empty())
	{
		content_type = "application/octet-stream";
	}

	auto response = std::make_shared<http::FileResponse>(
		full_path, false, 0, content_type
	);
	response->set_header(
		http::LAST_MODIFIED,
		http::http_date(stat_info.st_mtime)
	);
	if (!encoding.empty())
	{
		response->set_header(http::CONTENT_ENCODING, encoding);
	}

	return this->response(response);
}

__VIEWS_END__


__VIEWS_INTERNAL_BEGIN__

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
			if (!header_len.empty() && std::strtol(header_len.c_str(), nullptr, 10) != size)
			{
				result = true;
			}
			else if (time > header_time)
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

__VIEWS_INTERNAL_END__
