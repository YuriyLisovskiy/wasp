/**
 * http/mime/content_types.h
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 *
 * Commonly used content types definitsions.
 */

#pragma once

#include "./_def_.h"


__HTTP_MIME_BEGIN__

// application/*
inline constexpr const char* APPLICATION_JSON = "application/json";
inline constexpr const wchar_t* APPLICATION_JSON_L = L"application/json";

inline constexpr const char* APPLICATION_X_WWW_FORM_URLENCODED = "application/x-www-form-urlencoded";
inline constexpr const wchar_t* APPLICATION_X_WWW_FORM_URLENCODED_L = L"application/x-www-form-urlencoded";

inline constexpr const char* APPLICATION_OCTET_STREAM = "application/octet-stream";
inline constexpr const wchar_t* APPLICATION_OCTET_STREAM_L = L"application/octet-stream";

// multipart/*
inline constexpr const char* MULTIPART_FORM_DATA = "multipart/form-data";
inline constexpr const wchar_t* MULTIPART_FORM_DATA_L = L"multipart/form-data";

__HTTP_MIME_END__
