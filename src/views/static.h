/**
 * views/static.h
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 *
 * Purpose:
 * 	Views and functions for serving static files. These are only to be used
 * 	during development, and SHOULD NOT be used in a production setting.
 */

#pragma once

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "./view.h"


__VIEWS_BEGIN__

/// Serve static files below a given point in the directory structure.
class StaticView final : public View
{
private:
	collections::Dict<std::string, std::string>* _kwargs = nullptr;

public:
	explicit StaticView(conf::Settings* settings);

	void set_kwargs(collections::Dict<std::string, std::string>* kwargs);

	std::unique_ptr<http::IHttpResponse> get(http::HttpRequest* request, Args* args) final;
};

__VIEWS_END__


__VIEWS_INTERNAL_BEGIN__

/// Checks if something was modified since the user last downloaded it.
extern bool was_modified_since(const std::string& header, size_t time, size_t size);

__VIEWS_INTERNAL_END__
