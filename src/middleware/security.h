/**
 * middleware/security.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * TODO: docs
 */

#pragma once

// C++ libraries.
#include <vector>
#include <memory>

// Base libraries.
#include <xalwart.base/re/regex.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "./types.h"
#include "./base.h"


__MIDDLEWARE_BEGIN__

// TESTME: Security
// TODO: docs for 'Security'
class Security : public MiddlewareWithConstantSettings
{
public:
	static inline constexpr const char* NAME = "xw::middleware::Security";

	explicit inline Security(conf::Settings* settings) : MiddlewareWithConstantSettings(settings)
	{
		this->secure = this->settings->SECURE;
		for (auto& pattern : this->secure.REDIRECT_EXEMPT)
		{
			this->redirect_exempt.emplace_back(pattern);
		}
	}

	virtual Function operator() (const Function& next) const;

protected:
	conf::Secure secure;
	std::vector<re::Regex> redirect_exempt;

	virtual std::unique_ptr<http::abc::IHttpResponse> preprocess(http::Request* request) const;

	virtual std::unique_ptr<http::abc::IHttpResponse> postprocess(
		http::Request* request, http::abc::IHttpResponse* response
	) const;
};

__MIDDLEWARE_END__
