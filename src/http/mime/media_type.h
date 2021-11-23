/**
 * http/mime/media_type.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * Media types utilities.
 */

#pragma once

// C++ libraries.
#include <string>
#include <map>
#include <tuple>

// Base libraries.
#include <xalwart.base/string_utils.h>

// Module definitions.
#include "./_def_.h"


__HTTP_MIME_BEGIN__

// TESTME: ext_from_file_name
//
// Returns extension from file name,
//		i.e. 'foo.bar' - 'bar' will be returned.
//
// `file_name`: name to analyze.
extern std::string ext_from_file_name(const std::string& file_name);

// TESTME: ext_from_path
//
// Returns extension from file path,
//		i.e. 'path/to/foo.bar' - 'bar' will be returned.
//
// `path`: path to analyze.
extern std::string ext_from_path(const std::string& path);

// TESTME: guess_content_type
//
// Returns content type from file path.
//
// `_path`: path to analyze.
// `type`: guessed file type.
// `encoding`: guessed file encoding.
extern void guess_content_type(const std::string& _path, std::string& type, std::string& encoding);

// TESTME: _is_t_special
//
// The original implementation is in Golang 1.15.8: mime/grammar.go
inline bool _is_t_special(wchar_t c)
{
	std::wstring s = L"()<>@,;:\\\"/[]?=";
	return s.find(c) != std::wstring::npos;
}

// TESTME: _is_token_char
// Reports whether rune is in 'token' as defined by RFC
// 1521 and RFC 2045.
//
// The original implementation is in Golang 1.15.8: mime/grammar.go
inline bool _is_token_char(wchar_t c)
{
	// token := 1*<any (US-ASCII) CHAR except SPACE, CTLs, or tspecials>
	return c > 0x20 && c < 0x7f && !_is_t_special(c);
}

// TESTME: _is_not_token_char
//
// The original implementation is in Golang 1.15.8: mime/mediatype.go
inline bool _is_not_token_char(wchar_t c)
{
	return !_is_token_char(c);
}

// TESTME: _consume_token
// Consumes a token from the beginning of provided
// string, per RFC 2045 section 5.1 (referenced from 2183), and return
// the token consumed and the rest of the string. Returns {"", v} on
// failure to consume at least one character.
//
// The original implementation is in Golang 1.15.8: mime/mediatype.go
extern std::tuple<std::wstring, std::wstring> _consume_token(const std::wstring& s);

// TESTME: _consume_value
// Consumes a "value" per RFC 2045, where a value is
// either a 'token' or a 'quoted-string'.  On success, `consume_value`
// returns the value consumed (and de-quoted/escaped, if a
// quoted-string) and the rest of the string. On failure, returns
// {"", v}.
//
// The original implementation is in Golang 1.15.8: mime/mediatype.go
extern std::tuple<std::wstring, std::wstring> _consume_value(const std::wstring& s);

// TESTME: check_media_type_disposition
//
// The original implementation is in Golang 1.15.8: mime/mediatype.go
extern void _check_media_type_disposition(const std::wstring& s);

// TESTME: consume_media_parameter
//
// The original implementation is in Golang 1.15.8: mime/mediatype.go
extern std::tuple<std::wstring, std::wstring, std::wstring> _consume_media_parameter(const std::wstring& s);

// TESTME: parse_media_type
//
// Parses a media type value and any optional
// parameters, per RFC 1521. Media types are the values in
// Content-Type and Content-Disposition headers (RFC 2183).
// On success, parse_media_type returns the media type converted
// to lowercase and trimmed of white space map.
// The returned map, params, maps from the lowercase
// attribute to the attribute value with its case preserved.
//
// The original implementation is in Golang 1.15.8: mime/mediatype.go
extern std::tuple<std::wstring, std::map<std::wstring, std::wstring>, bool> parse_media_type(std::wstring content_type);

inline std::string _get_from_string_map(
	const std::map<std::string, std::string>& map, const std::string& key, const std::string& default_
)
{
	return map.contains(key) ? map.at(key) : default_;
}

// Dictionary which maps reduced archives' extensions to
// their full variants.
inline static const std::map<std::string, std::string> SUFFIX_MAP({
	{".svgz", ".svg.gz"},
	{".tgz", ".tar.gz"},
	{".taz", ".tar.gz"},
	{".tz", ".tar.gz"},
	{".tbz2", ".tar.bz2"},
	{".txz", ".tar.xz"}
});

// Dictionary which maps archives' extensions to
//	encoding names.
inline static const std::map<std::string, std::string> ENCODINGS_MAP({
	{".gz", "gzip"},
	{".Z", "compress"},
	{".bz2", "bzip2"},
	{".xz", "xz"},
});

// Before adding new types, make sure they are either registered with IANA,
// at http://www.iana.org/assignments/media-types
// or extensions, i.e. using the x- prefix
//
// If you add to these, please keep them sorted!
inline static const std::map<std::string, std::string> TYPES_MAP({
	{".3gp", "video/3gpp"},
	{".3gpp", "video/3gpp"},
	{".7z", "application/x-7z-compressed"},
	{".a", "application/octet-stream"},
	{".ai", "application/postscript"},
	{".aif", "audio/x-aiff"},
	{".aifc", "audio/x-aiff"},
	{".aiff", "audio/x-aiff"},
	{".asf", "video/x-ms-asf"},
	{".asx", "video/x-ms-asf"},
	{".atom", "application/atom+xml"},
	{".au", "audio/basic"},
	{".avi", "video/x-msvideo"},
	{".bat", "text/plain"},
	{".bcpio", "application/x-bcpio"},
	{".bin", "application/octet-stream"},
	{".bmp", "image/x-ms-bmp"},
	{".bz2", "application/octet-stream"},
	{".c", "text/plain"},
	{".cco", "application/x-cocoa"},

	// Multiple content types
	{".cdf", "application/x-cdf"},
	{".cdf", "application/x-netcdf"},

	{".cpio", "application/x-cpio"},
	{".crt", "application/x-x509-ca-cert"},
	{".csh", "application/x-csh"},
	{".css", "text/css"},
	{".csv", "text/csv"},
	{".deb", "application/octet-stream"},
	{".der", "application/x-x509-ca-cert"},
	{".dll", "application/octet-stream"},
	{".dmg", "application/octet-stream"},
	{".doc", "application/msword"},
	{".docx", "application/vnd.openxmlformats-officedocument.wordprocessingml.document"},
	{".dot", "application/msword"},
	{".dvi", "application/x-dvi"},
	{".ear", "application/java-archive"},
	{".eml", "message/rfc822"},
	{".eot", "application/octet-stream"},
	{".eps", "application/postscript"},
	{".etx", "text/x-setext"},
	{".exe", "application/octet-stream"},
	{".flv", "video/x-flv"},
	{".gif", "image/gif"},
	{".gtar", "application/x-gtar"},

	// Multiple content types
	{".gz", "application/gzip"},
//	{".gz", "application/x-gzip"},
//	{".gz", "application/x-gunzip"},
//	{".gz", "application/gzipped"},
//	{".gz", "application/gzip-compressed"},
//	{".gz", "application/application/x-compressed"},
//	{".gz", "application/x-compress"},
//	{".gz", "gzip/document"},
//	{".gz", "application/octet-stream"},

	{".h", "text/plain"},
	{".hdf", "application/x-hdf"},
	{".hqx", "application/mac-binhex40"},
	{".htc", "text/x-component"},
	{".htm", "text/html"},
	{".html", "text/html"},
	{".ico", "image/x-icon"},
	{".ief", "image/ief"},
	{".img", "application/octet-stream"},
	{".iso", "application/octet-stream"},
	{".jad", "text/vnd.sun.j2me.app-descriptor"},
	{".jade", "text/jade"},
	{".jar", "application/java-archive"},
	{".jardiff", "application/x-java-archive-diff"},
	{".jng", "image/x-jng"},
	{".jnlp", "application/x-java-jnlp-file"},
	{".jpe", "image/jpeg"},
	{".jpeg", "image/jpeg"},
	{".jpg", "image/jpeg"},
	{".js", "application/x-javascript"},
	{".json", "application/json"},
	{".kar", "audio/midi"},
	{".kml", "application/vnd.google-earth.kml+xml"},
	{".kmz", "application/vnd.google-earth.kmz"},
	{".ksh", "text/plain"},
	{".latex", "application/x-latex"},
	{".m1v", "video/mpeg"},
	{".m3u", "application/vnd.apple.mpegurl"},
	{".m3u8", "application/vnd.apple.mpegurl"},
	{".man", "application/x-troff-man"},
	{".me", "application/x-troff-me"},
	{".mht", "message/rfc822"},
	{".mhtml", "message/rfc822"},
	{".mid", "audio/midi"},
	{".midi", "audio/midi"},
	{".mif", "application/x-mif"},
	{".mjs", "application/javascript"},
	{".mml", "text/mathml"},
	{".mng", "video/x-mng"},
	{".mov", "video/quicktime"},
	{".movie", "video/x-sgi-movie"},
	{".mp2", "audio/mpeg"},
	{".mp3", "audio/mpeg"},
	{".mp4", "video/mp4"},
	{".mpa", "video/mpeg"},
	{".mpe", "video/mpeg"},
	{".mpeg", "video/mpeg"},
	{".mpg", "video/mpeg"},
	{".ms", "application/x-troff-ms"},
	{".msi", "application/octet-stream"},
	{".msm", "application/octet-stream"},
	{".msp", "application/octet-stream"},
	{".nc", "application/x-netcdf"},
	{".nws", "message/rfc822"},
	{".o", "application/octet-stream"},
	{".obj", "application/octet-stream"},
	{".oda", "application/oda"},
	{".odt", "application/vnd.oasis.opendocument.text"},
	{".ogg", "audio/ogg"},
	{".p12", "application/x-pkcs12"},
	{".p7c", "application/pkcs7-mime"},
	{".pbm", "image/x-portable-bitmap"},
	{".pdb", "application/x-pilot"},
	{".pdf", "application/pdf"},
	{".pem", "application/x-x509-ca-cert"},
	{".pfx", "application/x-pkcs12"},
	{".pgm", "image/x-portable-graymap"},
	{".pl", "application/x-perl"},
	{".pm", "application/x-perl"},
	{".png", "image/png"},
	{".pnm", "image/x-portable-anymap"},
	{".pot", "application/vnd.ms-powerpoint"},
	{".ppa", "application/vnd.ms-powerpoint"},
	{".ppm", "image/x-portable-pixmap"},
	{".pps", "application/vnd.ms-powerpoint"},
	{".ppt", "application/vnd.ms-powerpoint"},
	{".pptx", "application/vnd.openxmlformats-officedocument.presentationml.presentation"},
	{".prc", "application/x-pilot"},
	{".ps", "application/postscript"},
	{".pwz", "application/vnd.ms-powerpoint"},
	{".py", "text/x-python"},
	{".pyc", "application/x-python-code"},
	{".pyo", "application/x-python-code"},
	{".qt", "video/quicktime"},
	{".ra", "audio/x-realaudio"},
	{".ram", "application/x-pn-realaudio"},
	{".rar", "application/x-rar-compressed"},
	{".ras", "image/x-cmu-raster"},
	{".rdf", "application/xml"},
	{".rgb", "image/x-rgb"},
	{".roff", "application/x-troff"},
	{".rpm", "application/x-redhat-package-manager"},
	{".rss", "application/rss+xml"},
	{".rtf", "application/rtf"},
	{".rtx", "text/richtext"},
	{".run", "application/x-makeself"},
	{".sea", "application/x-sea"},
	{".sgm", "text/x-sgml"},
	{".sgml", "text/x-sgml"},
	{".sh", "application/x-sh"},
	{".shar", "application/x-shar"},
	{".shtml", "text/html"},
	{".sit", "application/x-stuffit"},
	{".snd", "audio/basic"},
	{".so", "application/octet-stream"},
	{".src", "application/x-wais-source"},
	{".sv4cpio", "application/x-sv4cpio"},
	{".sv4crc", "application/x-sv4crc"},
	{".svg", "image/svg+xml"},
	{".swf", "application/x-shockwave-flash"},
	{".t", "application/x-troff"},
	{".tar", "application/x-tar"},
	{".tcl", "application/x-tcl"},
	{".tex", "application/x-tex"},
	{".texi", "application/x-texinfo"},
	{".texinfo", "application/x-texinfo"},
	{".tif", "image/tiff"},
	{".tiff", "image/tiff"},
	{".tk", "application/x-tcl"},
	{".tr", "application/x-troff"},
	{".tsv", "text/tab-separated-values"},
	{".txt", "text/plain"},
	{".ustar", "application/x-ustar"},
	{".vcf", "text/x-vcard"},
	{".war", "application/java-archive"},
	{".wav", "audio/x-wav"},
	{".wbmp", "image/vnd.wap.wbmp"},
	{".webm", "video/webm"},
	{".wiz", "application/msword"},
	{".wml", "text/vnd.wap.wml"},
	{".wmlc", "application/vnd.wap.wmlc"},
	{".wmv", "video/x-ms-wmv"},
	{".wsdl", "application/xml"},
	{".xbm", "image/x-xbitmap"},
	{".xhtml", "application/xhtml+xml"},
	{".xlb", "application/vnd.ms-excel"},

	// Multiple content types
	{".xls", "application/excel"},
	{".xls", "application/vnd.ms-excel"},

	{".xlsx", "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"},
	{".xml", "text/xml"},
	{".xpdl", "application/xml"},
	{".xpi", "application/x-xpinstall"},
	{".xpm", "image/x-xpixmap"},
	{".xsl", "application/xml"},
	{".xwd", "image/x-xwindowdump"},
	{".xz", "application/x-xz"},

	// Multiple content types
	{".Z", "application/x-z"},
//	{".Z", "application/x-compress"},
//	{".Z", "application/x-compressed"},
//	{".Z", "application/gzip"},
//	{".Z", "application/x-gzip"},
//	{".Z", "application/z"},

	{".zip", "application/zip"},
});

__HTTP_MIME_END__
