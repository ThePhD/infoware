// SPDX-License-Identifier: CC0-1.0
// infoware - C++ System information Library


#ifndef _WIN32


#include "infoware/detail/winstring.hpp"


std::string iware::detail::narrowen_winstring(const wchar_t*) {
	return {};
}

std::string iware::detail::narrowen_bstring(const wchar_t*) {
	return {};
}


#endif
