/**
 * http/url.cpp
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include "./url.h"

// Base libraries.
#include <xalwart.base/exceptions.h>
#include <xalwart.base/encoding.h>


__HTTP_BEGIN__

std::string Query::encode() const
{
	std::string buf;
	auto keys = this->keys();
	std::sort(keys.begin(), keys.end());
	for (const auto& k : keys)
	{
		auto vs = this->getlist(k);
		auto key_escaped = query_escape(k);
		for (const auto& v : vs)
		{
			if (!buf.empty())
			{
				buf += '&';
			}

			buf += key_escaped + "=" + query_escape(v);
		}
	}

	return buf;
}

Query parse_query(std::string_view query)
{
	Query m;
	while (!query.empty())
	{
		auto key = query;
		auto i = key.find('&');
		if (i == std::string::npos)
		{
			i = key.find(';');
		}

		if (i != std::string::npos)
		{
			query = key.substr(i + 1);
			key = key.substr(0, i);
		}
		else
		{
			query = "";
		}

		if (key.empty())
		{
			continue;
		}

		std::string value;
		i = key.find('=');
		if (i != std::string::npos)
		{
			value = key.substr(i + 1);
			key = key.substr(0, i);
		}

		key = query_unescape(std::string(key));
		value = query_unescape(value);
		m.add(std::string(key), value);
	}

	return m;
}

void URL::set_path(const std::string& p)
{
	this->path = internal::unescape(p, internal::EscapeMode::EncodePath);
	this->raw_path = p == internal::escape(this->path, internal::EscapeMode::EncodePath) ? "" : p;
}

std::string URL::escaped_path() const
{
	if (!this->raw_path.empty() && internal::valid_encoded(this->raw_path, internal::EscapeMode::EncodePath))
	{
		try
		{
			auto p = internal::unescape(this->raw_path, internal::EscapeMode::EncodePath);
			if (p == this->path)
			{
				return this->raw_path;
			}
		}
		catch (const EscapeError&)
		{
		}
	}

	if (this->path == "*")
	{
		return "*"; // don't escape
	}

	return internal::escape(this->path, internal::EscapeMode::EncodePath);
}

void URL::set_fragment(const std::string& f)
{
	this->fragment = internal::unescape(f, internal::EscapeMode::EncodeFragment);
	this->raw_fragment = f == internal::escape(this->fragment, internal::EscapeMode::EncodeFragment) ? "" : f;
}

std::string URL::escaped_fragment() const
{
	auto is_valid = internal::valid_encoded(this->raw_fragment, internal::EscapeMode::EncodeFragment);
	if (!this->raw_fragment.empty() && is_valid)
	{
		try
		{
			auto f = internal::unescape(this->raw_fragment, internal::EscapeMode::EncodeFragment);
			if (f == this->fragment)
			{
				return this->raw_fragment;
			}
		}
		catch(const EscapeError&)
		{
		}
	}

	return internal::escape(this->fragment, internal::EscapeMode::EncodeFragment);
}

std::string URL::str() const
{
	std::string result;
	if (!this->scheme.empty())
	{
		result += this->scheme + ":";
	}

	if (!this->opaque.empty())
	{
		result += this->opaque;
	}
	else
	{
		if (!this->scheme.empty() || !this->host.empty() || (bool)this->user)
		{
			if (!this->host.empty() || !this->path.empty() || (bool)this->user)
			{
				result += "//";
			}

			if ((bool)this->user)
			{
				result += this->user.str() + "@";
			}

			if (!this->host.empty())
			{
				result += internal::escape(this->host, internal::EscapeMode::EncodeHost);
			}
		}

		auto escaped_path = this->escaped_path();
		if (!escaped_path.empty() && escaped_path[0] != '/' && !this->host.empty()) {
			result += "/";
		}

		if (result.empty())
		{
			// RFC 3986 §4.2
			// A path segment that contains a colon character (e.g., "this:that")
			// cannot be used as the first segment of a relative-path reference, as
			// it would be mistaken for a scheme name. Such a segment must be
			// preceded by a dot-segment (e.g., "./this:that") to make a relative-
			// path reference.
			auto i = escaped_path.find(':');
			if (i != std::string::npos && escaped_path.substr(0, i).find('/') == std::string::npos)
			{
				result += "./";
			}
		}

		result += escaped_path;
	}

	if (this->force_query || !this->raw_query.empty())
	{
		result += "?" + this->raw_query;
	}

	if (!this->fragment.empty())
	{
		result += "#" + this->escaped_fragment();
	}

	return result;
}

std::string URL::redacted() const
{
	auto url = *this;
	if (!url.user.password.empty())
	{
		url.user.password = "xxxxx";
	}

	return url.str();
}

URL URL::resolve_reference(const URL& ref) const
{
	auto url = ref;
	if (ref.scheme.empty())
	{
		url.scheme = this->scheme;
	}

	if (!ref.scheme.empty() || !ref.host.empty() || (bool)ref.user)
	{
		// The "absoluteURI" or "net_path" cases.
		// We can ignore the error from 'set_path' since we know we provided a
		// validly-escaped path.
		url.set_path(internal::resolve_path(ref.escaped_path(), ""));
		return url;
	}

	if (!ref.opaque.empty())
	{
		url.user = URL::UserInfo{};
		url.host = "";
		url.path = "";
		return url;
	}

	if (ref.path.empty() && ref.raw_query.empty())
	{
		url.raw_query = this->raw_query;
		if (ref.fragment.empty())
		{
			url.fragment = this->fragment;
			url.raw_fragment = this->raw_fragment;
		}
	}

	// The "abs_path" or "rel_path" cases.
	url.host = this->host;
	url.user = this->user;
	url.set_path(internal::resolve_path(this->escaped_path(), ref.escaped_path()));
	return url;
}

std::string URL::request_uri() const
{
	auto result = this->opaque;
	if (result.empty())
	{
		result = this->escaped_path();
		if (result.empty())
		{
			result = "/";
		}
	}
	else
	{
		if (result.starts_with("//"))
		{
			result = this->scheme + ":" + result;
		}
	}

	if (this->force_query || !this->raw_query.empty())
	{
		result += "?" + this->raw_query;
	}

	return result;
}

URL parse_url(const std::string& raw_url)
{
	auto [u, frag] = internal::split(raw_url, '#', true);
	auto url = internal::parse_url(u, false);
	if (frag.empty())
	{
		return url;
	}

	url.set_fragment(frag);
	return url;
}

__HTTP_END__


__HTTP_INTERNAL_BEGIN__

bool valid_optional_port(const std::string& port)
{
	if (port.empty())
	{
		return true;
	}

	if (port[0] != ':')
	{
		return false;
	}

	auto port_subs = port.substr(1);
	return std::all_of(
		port.begin() + 1, port.end(), [](const auto& c) -> bool { return c >= '0' && c <= '9'; }
	);
}

std::pair<std::string, std::string> split_host_port(const std::string& host_port)
{
	std::pair<std::string, std::string> result{host_port, ""};
	auto colon = result.first.find_last_of(':');
	if (colon != std::string::npos && valid_optional_port(result.first.substr(colon)))
	{
		result.second = result.first.substr(colon + 1);
		result.first = result.first.substr(0, colon);
	}

	if (result.first.starts_with("[") && result.first.ends_with("]"))
	{
		result.first = result.first.substr(1, result.first.size() - 2);
	}

	return result;
}

bool should_escape(wchar_t c, EscapeMode mode)
{
	// §2.3 Unreserved characters (alphanum)
	if ('a' <= c && c <= 'z' || 'A' <= c && c <= 'Z' || '0' <= c && c <= '9')
	{
		return false;
	}

	if (mode == EscapeMode::EncodeHost || mode == EscapeMode::EncodeZone)
	{
		// §3.2.2 Host allows
		// sub-delims = "!" / "$" / "&" / "'" / "(" / ")" / "*" / "+" / "," / ";" / "="
		// as part of reg-name.
		// We add : because we include :port as part of host.
		// We add [ ] because we include [ipv6]:port as part of host.
		// We add < > because they're the only characters left that
		// we could possibly allow, and Parse will reject them if we
		// escape them (because hosts can't use %-encoding for
		// ASCII bytes).
		switch (c)
		{
			case '!':
			case '$':
			case '&':
			case '\'':
			case '(':
			case ')':
			case '*':
			case '+':
			case ',':
			case ';':
			case '=':
			case ':':
			case '[':
			case ']':
			case '<':
			case '>':
			case '"':
				return false;
		}
	}

	switch (c)
	{
		// §2.3 Unreserved characters (mark)
		case '-':
		case '_':
		case '.':
		case '~':
			return false;

		// §2.2 Reserved characters (reserved)
		case '$':
		case '&':
		case '+':
		case ',':
		case '/':
		case ':':
		case ';':
		case '=':
		case '?':
		case '@':
			// Different sections of the URL allow a few of
			// the reserved characters to appear unescaped.
			switch (mode)
			{
				case EscapeMode::EncodePath: // §3.3
					// The RFC allows : @ & = + $ but saves / ; , for assigning
					// meaning to individual path segments. This package
					// only manipulates the path as a whole, so we allow those
					// last three as well. That leaves only ? to escape.
					return c == '?';
				case EscapeMode::EncodePathSegment: // §3.3
					// The RFC allows : @ & = + $ but saves / ; , for assigning
					// meaning to individual path segments.
					return c == '/' || c == ';' || c == ',' || c == '?';
				case EscapeMode::EncodeUserPassword: // §3.2.1
					// The RFC allows ';', ':', '&', '=', '+', '$', and ',' in
					// userinfo, so we must escape only '@', '/', and '?'.
					// The parsing of userinfo treats ':' as special so we must escape
					// that too.
					return c == '@' || c == '/' || c == '?' || c == ':';
				case EscapeMode::EncodeQueryComponent: // §3.4
					// The RFC reserves (so we must escape) everything.
					return true;
				case EscapeMode::EncodeFragment: // §4.1
					// The RFC text is silent but the grammar allows
					// everything, so escape nothing.
					return false;
				default:
					break;
			}
	}

	if (mode == EscapeMode::EncodeFragment)
	{
		// RFC 3986 §2.2 allows not escaping sub-delims. A subset of sub-delims are
		// included in reserved from RFC 2396 §2.2. The remaining sub-delims do not
		// need to be escaped. To minimize potential breakage, we apply two restrictions:
		// (1) we always escape sub-delims outside of the fragment, and (2) we always
		// escape single quote to avoid breaking callers that had previously assumed that
		// single quotes would be escaped. See issue #19917.
		switch (c)
		{
			case '!':
			case '(':
			case ')':
			case '*':
				return false;
		}
	}

	// Everything else must be escaped.
	return true;
}

std::string escape(const std::string& string, EscapeMode mode)
{
	auto s = str::string_to_wstring(string);
	unsigned int space_count = 0, hex_count = 0;
	for (auto c : s)
	{
		if (should_escape(c, mode))
		{
			if (c == ' ' && mode == EscapeMode::EncodeQueryComponent)
			{
				space_count++;
			}
			else
			{
				hex_count++;
			}
		}
	}

	if (space_count == 0 && hex_count == 0)
	{
		return str::wstring_to_string(s);
	}

	auto required = s.size() + 2 * hex_count;
	auto* t = new wchar_t[required];
	try
	{
		if (hex_count == 0)
		{
			for (size_t i = 0; i < s.size(); i++)
			{
				t[i] = s[i] == ' ' ? '+' : s[i];
			}

			auto result = str::wstring_to_string(std::wstring(t));
			delete[] t;
			return result;
		}

		size_t j = 0;
		for (auto c : s)
		{
			if (c == ' ' && mode == EscapeMode::EncodeQueryComponent)
			{
				t[j] = '+';
				j++;
			}
			else if (should_escape(c, mode))
			{
				t[j] = '%';
				t[j + 1] = UPPER_HEX[c >> 4];
				t[j + 2] = UPPER_HEX[c & 15];
				j += 3;
			}
			else
			{
				t[j] = c;
				j++;
			}
		}

		auto result = str::wstring_to_string(std::wstring(t));
		delete[] t;
		return result;
	}
	catch (const std::exception& exc)
	{
		delete[] t;
		throw exc;
	}
}

std::string unescape(const std::string& string, EscapeMode mode)
{
	auto s = str::string_to_wstring(string);

	// Count %, check that they're well-formed.
	size_t n = 0;
	bool has_plus = false;
	for (size_t i = 0; i < s.size(); i++)
	{
		switch (s[i])
		{
			case '%':
				n++;
				if (i + 2 >= s.size() || !encoding::is_hex(s[i + 1]) || !encoding::is_hex(s[i + 2]))
				{
					s = s.substr(i);
					if (s.size() > 3)
					{
						s = s.substr(0, 3);
					}

					throw EscapeError(str::wstring_to_string(s), _ERROR_DETAILS_);
				}

				// Per https://tools.ietf.org/html/rfc3986#page-21
				// in the host component %-encoding can only be used
				// for non-ASCII bytes.
				// But https://tools.ietf.org/html/rfc6874#section-2
				// introduces %25 being allowed to escape a percent sign
				// in IPv6 scoped-address literals. Yay.
				if (mode == EscapeMode::EncodeHost && encoding::unhex(s[i + 1]) < 8 && s.substr(i, 3) != L"%25")
				{
					throw EscapeError(str::wstring_to_string(s.substr(i, 3)), _ERROR_DETAILS_);
				}

				if (mode == EscapeMode::EncodeZone)
				{
					// RFC 6874 says basically "anything goes" for zone identifiers
					// and that even non-ASCII can be redundantly escaped,
					// but it seems prudent to restrict %-escaped bytes here to those
					// that are valid host name bytes in their unescaped form.
					// That is, you can use escaping in the zone identifier but not
					// to introduce bytes you couldn't just write directly.
					// But Windows puts spaces here! Yay.
					auto v = (encoding::unhex(s[i + 1]) << 4) | encoding::unhex(s[i + 2]);
					if (s.substr(i, 3) != L"%25" && v != ' ' && should_escape(v, EscapeMode::EncodeHost))
					{
						throw EscapeError(str::wstring_to_string(s.substr(i, 3)), _ERROR_DETAILS_);
					}
				}

				i += 3;
				break;
			case '+':
				has_plus = mode == EscapeMode::EncodeQueryComponent;
				i++;
				break;
			default:
				if (
					(mode == EscapeMode::EncodeHost || mode == EscapeMode::EncodeZone) &&
					s[i] < 0x80 && should_escape(s[i], mode)
				)
				{
					throw EscapeError("invalid host: " + str::wstring_to_string(s.substr(i, 1)), _ERROR_DETAILS_);
				}

				i++;
				break;
		}
	}

	if (n == 0 && !has_plus)
	{
		return str::wstring_to_string(s);
	}

	std::wstring t;
	t.reserve(s.size() - 2 * n);
	for (size_t i = 0; i < s.size(); i++)
	{
		switch (s[i])
		{
			case '%':
				t += (wchar_t)(encoding::unhex(s[i + 1]) << 4 | encoding::unhex(s[i + 2]));
				i += 2;
				break;
			case '+':
				if (mode == EscapeMode::EncodeQueryComponent)
				{
					t += ' ';
				}
				else
				{
					t += '+';
				}
				break;
			default:
				t += s[i];
				break;
		}
	}

	return str::wstring_to_string(t);
}

bool valid_encoded(const std::string& s, EscapeMode mode)
{
	for (char c : s)
	{
		// RFC 3986, Appendix A.
		// pchar = unreserved / pct-encoded / sub-delims / ":" / "@".
		// shouldEscape is not quite compliant with the RFC,
		// so we check the sub-delims ourselves and let
		// shouldEscape handle the others.
		switch (c)
		{
			case '!':
			case '$':
			case '&':
			case '\'':
			case '(':
			case ')':
			case '*':
			case '+':
			case ',':
			case ';':
			case '=':
			case ':':
			case '@':
				// ok
				break;
			case '[':
			case ']':
				// ok - not specified in RFC 3986 but left alone by modern browsers
				break;
			case '%':
				// ok - percent encoded, will decode
				break;
			default:
				if (should_escape(c, mode))
				{
					return false;
				}

				break;
		}
	}

	return true;
}

std::pair<std::string, std::string> split(const std::string& s, char sep, bool cut_sep)
{
	auto i = s.find(sep);
	if (i == std::string::npos)
	{
		return {s, ""};
	}

	if (cut_sep)
	{
		return {s.substr(0, i), s.substr(i + 1)};
	}

	return {s.substr(0, i), s.substr(i)};
}

std::pair<std::string, std::string> get_scheme(const std::string& raw_url)
{
	for (size_t i = 0; i < raw_url.size(); i++)
	{
		auto c = raw_url[i];
		if ('a' <= c && c <= 'z' || 'A' <= c && c <= 'Z')
		{
			// do nothing
		}
		else if ('0' <= c && c <= '9' || c == '+' || c == '-' || c == '.')
		{
			if (i == 0)
			{
				return {"", raw_url};
			}
		}
		else if (c == ':')
		{
			if (i == 0)
			{
				throw ParseError("missing protocol scheme", _ERROR_DETAILS_);
			}

			return {raw_url.substr(0, i), raw_url.substr(i + 1)};
		}
		else
		{
			// we have encountered an invalid character,
			// so there is no valid scheme
			return {"", raw_url};
		}
	}

	return {"", raw_url};
}

std::string parse_host(std::string host)
{
	if (host.starts_with('['))
	{
		// Parse an IP-Literal in RFC 3986 and RFC 6874.
		// E.g., "[fe80::1]", "[fe80::1%25en0]", "[fe80::1]:80".
		auto i = host.find_last_of(']');
		if (i == std::string::npos)
		{
			throw ParseError("missing ']' in host", _ERROR_DETAILS_);
		}

		auto colon_port = host.substr(i + 1);
		if (!valid_optional_port(colon_port))
		{
			throw ParseError("invalid port " + colon_port + " after host", _ERROR_DETAILS_);
		}

		// RFC 6874 defines that %25 (%-encoded percent) introduces
		// the zone identifier, and the zone identifier can use basically
		// any %-encoding it likes. That's different from the host, which
		// can only %-encode non-ASCII bytes.
		// We do impose some restrictions on the zone, to avoid stupidity
		// like newlines.
		auto zone = host.substr(0, i).find("%25");
		if (zone != std::string::npos)
		{
			auto host1 = unescape(host.substr(0, zone), EscapeMode::EncodeHost);
			auto host2 = unescape(host.substr(zone, i - zone), EscapeMode::EncodeZone);
			auto host3 = unescape(host.substr(i), EscapeMode::EncodeHost);
			return host1 + host2 + host3;
		}
	}
	else
	{
		auto i = host.find_last_of(':');
		if (i != std::string::npos)
		{
			auto colon_port = host.substr(i);
			if (!valid_optional_port(colon_port))
			{
				throw ParseError(
					"invalid port " + colon_port + " after host", _ERROR_DETAILS_
				);
			}
		}
	}

	host = unescape(host, EscapeMode::EncodeHost);
	return host;
}

bool valid_user_info(const std::string& s)
{
	return std::all_of(s.begin(), s.end(), [](const char& r) -> bool {
		if ('A' <= r && r <= 'Z')
		{
			return true;
		}

		if ('a' <= r && r <= 'z')
		{
			return true;
		}

		if ('0' <= r && r <= '9')
		{
			return true;
		}

		switch (r)
		{
			case '-':
			case '.':
			case '_':
			case ':':
			case '~':
			case '!':
			case '$':
			case '&':
			case '\'':
			case '(':
			case ')':
			case '*':
			case '+':
			case ',':
			case ';':
			case '=':
			case '%':
			case '@':
				return true;
			default:
				return false;
		}
	});
}

std::string resolve_path(const std::string& base, const std::string& ref)
{
	std::string full;
	if (ref.empty())
	{
		full = base;
	}
	else if (ref[0] != '/')
	{
		auto i = base.find_last_of('/');
		full = base.substr(0, i + 1) + ref;
	}
	else
	{
		full = ref;
	}

	if (full.empty())
	{
		return "";
	}

	auto src = str::split(full, '/');
	std::vector<std::string> dst;
	for (const auto& elem : src)
	{
		if (elem == ".")
		{
			// drop
		}
		else if (elem == "..")
		{
			if (!dst.empty())
			{
				dst.pop_back();
			}
		}
		else
		{
			dst.push_back(elem);
		}
	}

	const auto& last = src.back();
	if (last == "." || last == "..")
	{
		// Add final slash to the joined path.
		dst.emplace_back("");
	}

	return "/" + str::ltrim(str::join("/", dst.begin(), dst.end()), '/');
}

std::pair<URL::UserInfo, std::string> parse_authority(const std::string& authority)
{
	auto i = authority.find_last_of('@');
	std::string host;
	if (i == std::string::npos)
	{
		host = parse_host(authority);
	}
	else
	{
		host = parse_host(authority.substr(i + 1));
	}

	if (i == std::string::npos)
	{
		return {{}, host};
	}

	auto user_info = authority.substr(0, i);
	if (!valid_user_info(user_info))
	{
		throw ParseError("invalid user_info", _ERROR_DETAILS_);
	}

	URL::UserInfo user;
	if (!str::contains(user_info, ':'))
	{
		user_info = unescape(user_info, EscapeMode::EncodeUserPassword);
		user.username = user_info;
	}
	else
	{
		auto [username, password] = split(user_info, ':', true);
		username = unescape(username, EscapeMode::EncodeUserPassword);
		password = unescape(password, EscapeMode::EncodeUserPassword);
		user.username = username;
		user.password = password;
	}

	return {user, host};
}

URL parse_url(const std::string& raw_url, bool via_request)
{
	if (string_contains_ctl_byte(raw_url))
	{
		throw ParseError("invalid control character in URL", _ERROR_DETAILS_);
	}

	if (raw_url.empty() && via_request)
	{
		throw ParseError("empty url");
	}

	URL url;
	if (raw_url == "*")
	{
		url.path = "*";
		return url;
	}

	// Split off possible leading "http:", "mailto:", etc.
	// Cannot contain escaped characters.
	auto [scheme, rest] = get_scheme(raw_url);
	url.scheme = str::to_lower(scheme);
	if (rest.ends_with("?") && str::count(rest, '?') == 1)
	{
		url.force_query = true;
		rest = rest.substr(0, rest.size() - 1);
	}
	else
	{
		auto [new_rest, raw_query] = split(rest, '?', true);
		rest = new_rest;
		url.raw_query = raw_query;
	}

	if (!rest.starts_with('/'))
	{
		if (!url.scheme.empty())
		{
			// We consider rootless paths per RFC 3986 as opaque.
			url.opaque = rest;
			return url;
		}

		if (via_request)
		{
			throw ParseError("invalid URI for request", _ERROR_DETAILS_);
		}

		// Avoid confusion with malformed schemes, like cache_object:foo/bar.
		// See golang.org/issue/16822.
		//
		// RFC 3986, §3.3:
		// In addition, a URI reference (Section 4.1) may be a relative-path reference,
		// in which case the first path segment cannot contain a colon (":") character.
		auto colon = rest.find(':');
		auto slash = rest.find('/');
		if (colon != std::string::npos && (slash == std::string::npos || colon < slash))
		{
			throw ParseError("first path segment in URL cannot contain colon", _ERROR_DETAILS_);
		}
	}

	if ((!url.scheme.empty() || !via_request && !rest.starts_with("///")) && rest.starts_with("//"))
	{
		auto [authority, new_rest] = split(rest.substr(2), '/', false);
		rest = new_rest;
		auto [user, host] = parse_authority(authority);
		url.user = user;
		url.host = host;
	}

	// Set 'path' and, optionally, 'raw_path'.
	// 'raw_path' is a hint of the encoding of 'path'. We don't want to set it if
	// the default escaping of 'path' is equivalent, to help make sure that people
	// don't rely on it in general.
	url.set_path(rest);
	return url;
}

__HTTP_INTERNAL_END__
