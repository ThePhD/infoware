// SPDX-License-Identifier: CC0-1.0
// infoware - C++ System information Library


#ifdef _WIN32


#include "infoware/detail/winstring.hpp"
#include <cwchar>
#include <string>
#define WIN32_LEAN_AND_MEAN
#include <wbemidl.h>
#include <windows.h>


static std::string transcode_from_wide(const wchar_t* wstr, std::size_t wstr_size) {
	std::string ret;
	// convert even embedded NUL
	if(const auto len = WideCharToMultiByte(CP_UTF8, 0, wstr, static_cast<int>(wstr_size), nullptr, 0, 0, 0)) {
		ret.resize(len, '\0');
		if(!WideCharToMultiByte(CP_UTF8, 0, wstr, static_cast<int>(wstr_size), &ret[0], len, 0, 0))
			ret.clear();
	}
	return ret;
}

std::string iware::detail::narrowen_winstring(const wchar_t* wstr) {
	if(!wstr)
		return {};

	return transcode_from_wide(wstr, std::wcslen(wstr));
}

std::string iware::detail::narrowen_bstring(const wchar_t* bstr) {
	if(!bstr)
		return {};

	return transcode_from_wide(bstr, SysStringLen(const_cast<BSTR>(bstr)));
}


#endif
