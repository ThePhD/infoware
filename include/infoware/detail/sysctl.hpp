// SPDX-License-Identifier: CC0-1.0
// infoware - C++ System information Library


#pragma once


#include <infoware/linkage.hpp>

#include <cstdint>
#include <string>
#include <utility>


namespace iware {
	namespace detail {
		/// https://github.com/ThePhD/infoware/issues/13
		INFOWARE_API_LINKAGE_INTERNAL std::string sysctl(const char* name);
		INFOWARE_API_LINKAGE_INTERNAL std::string sysctl(int mib_0, int mib_1);

		INFOWARE_API_LINKAGE_INTERNAL std::pair<bool, std::uint64_t> deconstruct_sysctl_int(const std::string& data);
	}  // namespace detail
}  // namespace iware
