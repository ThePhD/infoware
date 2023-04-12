// SPDX-License-Identifier: CC0-1.0
// infoware - C++ System information Library


#pragma once


#include <infoware/linkage.hpp>

#include <cstdint>


namespace iware {
	namespace detail {
		INFOWARE_API_LINKAGE_INTERNAL void cpuid(std::int32_t (&out)[4], std::int32_t x);

		INFOWARE_API_LINKAGE_INTERNAL std::uint64_t xgetbv(std::uint32_t x);
	}  // namespace detail
}  // namespace iware
