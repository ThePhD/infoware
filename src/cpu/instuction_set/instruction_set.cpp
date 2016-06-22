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

#include <infoware/cpu.hpp>
#include <infoware/detail/cpuid.hpp>

std::vector<iware::cpu::instruction_set_t> supported_instructions() {
	int info[4];
	iware::detail::cpuid_intrinsic(info, 0);
	int idinfo = info[0];

	iware::detail::cpuid_intrinsic(info, 0x80000000);
	uint32_t extendedidinfo = info[0];
}