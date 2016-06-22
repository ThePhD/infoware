// infoware - C++ System information Library
//
// Written in 2016 by nabijaczleweli <nabijaczleweli@gmail.com> and ThePhD <phdofthehouse@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>


#include "infoware/cpu.hpp"
#include "infoware/detail/cpuid.hpp"


std::string iware::cpu::vendor_id() {
	int32_t CPUInfo[4];
	char name[13];

	detail::cpuid_intrinsic(CPUInfo, 0);
	memcpy(name + 0, &CPUInfo[1], 4);
	memcpy(name + 4, &CPUInfo[3], 4);
	memcpy(name + 8, &CPUInfo[2], 4);
	name[12] = '\0';

	return name;
}
