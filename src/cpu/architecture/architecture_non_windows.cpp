// SPDX-License-Identifier: CC0-1.0
// infoware - C++ System information Library


#ifndef _WIN32


#include "infoware/cpu.hpp"
#include <cstring>
#include <sys/utsname.h>


// https://github.com/karelzak/util-linux/blob/master/sys-utils/lscpu.c
iware::cpu::architecture_t iware::cpu::architecture() noexcept {
	utsname buf;

	if(uname(&buf) == -1)
		return iware::cpu::architecture_t::unknown;

	if(!strcmp(buf.machine, "x86_64"))
		return iware::cpu::architecture_t::x64;
	else if(strstr(buf.machine, "arm") == buf.machine)
		return iware::cpu::architecture_t::arm;
	else if(!strcmp(buf.machine, "ia64") || !strcmp(buf.machine, "IA64"))
		return iware::cpu::architecture_t::itanium;
	else if(!strcmp(buf.machine, "i686"))
		return iware::cpu::architecture_t::x86;
	else
		return iware::cpu::architecture_t::unknown;
}


#endif
