// SPDX-License-Identifier: CC0-1.0
// infoware - C++ System information Library


#ifdef _WIN32


#include "infoware/detail/cpuid.hpp"
#include <cstdint>
#include <intrin.h>


void iware::detail::cpuid(std::int32_t (&out)[4], std::int32_t x) {
	__cpuidex(out, x, 0);
}

std::uint64_t iware::detail::xgetbv(std::uint32_t x) {
	return _xgetbv(x);
}


#endif
