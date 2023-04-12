// SPDX-License-Identifier: CC0-1.0
// infoware - C++ System information Library


#pragma once


#include <infoware/linkage.hpp>

#include <string>


namespace iware {
	namespace detail {
		INFOWARE_API_LINKAGE_INTERNAL std::string narrowen_bstring(const wchar_t* wstr);
		INFOWARE_API_LINKAGE_INTERNAL std::string narrowen_winstring(const wchar_t* wstr);
	}  // namespace detail
}  // namespace iware
