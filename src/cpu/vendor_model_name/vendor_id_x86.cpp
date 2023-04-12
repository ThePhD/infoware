// SPDX-License-Identifier: CC0-1.0
// infoware - C++ System information Library


#include "infoware/platform.hpp"
#if INFOWARE_X86


#include <cstring>
#include <infoware/cpu.hpp>
#include <infoware/detail/cpuid.hpp>


std::string iware::cpu::vendor_id() {
	std::int32_t info[4];
	char name[13];

	iware::detail::cpuid(info, 0);
	std::memcpy(name + 0, &info[1], 4);
	std::memcpy(name + 4, &info[3], 4);
	std::memcpy(name + 8, &info[2], 4);
	name[12] = '\0';

	return name;
}


#endif /* INFOWARE_X86 */
