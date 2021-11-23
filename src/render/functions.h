/**
 * render/functions.h
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 *
 * Purpose: TODO
 */

#pragma once

// C++ libraries.
#include <string>

// Base libraries.
#include <xalwart.base/interfaces/render.h>

// Module definitions.
#include "./_def_.h"

// Render libraries.
#include "../urls/pattern.h"


__RENDER_BEGIN__

// TESTME: make_static_function
// TODO: docs for 'make_static_function'
extern render::ILibrary::Function make_static_function(const std::string& prefix);

// TESTME: make_url_function
// TODO: docs for 'make_url_function'
extern render::ILibrary::Function make_url_function(const std::vector<std::shared_ptr<urls::IPattern>>& patterns);

__RENDER_END__
