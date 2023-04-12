// SPDX-License-Identifier: CC0-1.0
// infoware - C++ System information Library


#ifdef _WIN32


#include "infoware/cpu.hpp"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>


// https://msdn.microsoft.com/en-us/library/windows/desktop/ms724958(v=vs.85).aspx
iware::cpu::architecture_t iware::cpu::architecture() noexcept {
	SYSTEM_INFO sysinfo;
	GetNativeSystemInfo(&sysinfo);

	switch(sysinfo.wProcessorArchitecture) {
		case PROCESSOR_ARCHITECTURE_AMD64:
			return iware::cpu::architecture_t::x64;
		case PROCESSOR_ARCHITECTURE_ARM:
			return iware::cpu::architecture_t::arm;
		case PROCESSOR_ARCHITECTURE_IA64:
			return iware::cpu::architecture_t::itanium;
		case PROCESSOR_ARCHITECTURE_INTEL:
			return iware::cpu::architecture_t::x86;
		default:
			return iware::cpu::architecture_t::unknown;
	}
}


#endif
