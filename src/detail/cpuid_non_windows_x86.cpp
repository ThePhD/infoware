// SPDX-License-Identifier: CC0-1.0
// infoware - C++ System information Library


#ifndef _WIN32

#include "infoware/platform.hpp"
#if INFOWARE_X86

#include "infoware/detail/cpuid.hpp"
#include <cpuid.h>
#include <cstdint>


void iware::detail::cpuid(std::int32_t (&out)[4], std::int32_t x) {
	__cpuid_count(x, 0, out[0], out[1], out[2], out[3]);
}

std::uint64_t iware::detail::xgetbv(std::uint32_t index) {
	std::uint32_t eax, edx;
	__asm__ __volatile__("xgetbv" : "=a"(eax), "=d"(edx) : "c"(index));
	return ((uint64_t)edx << 32) | eax;
}


#endif /* INFOWARE_X86 */
#endif /* _WIN32 */
