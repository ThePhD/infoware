// SPDX-License-Identifier: CC0-1.0
// infoware - C++ System information Library


#ifdef _WIN32


#include "infoware/cpu.hpp"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>


std::uint64_t iware::cpu::frequency() noexcept {
	HKEY hkey;
	if(RegOpenKeyExA(HKEY_LOCAL_MACHINE, R"(HARDWARE\DESCRIPTION\System\CentralProcessor\0)", 0, KEY_READ, &hkey)) {
		// Fallback
		LARGE_INTEGER freq;
		QueryPerformanceFrequency(&freq);
		return freq.QuadPart * 1'000;
	}

	DWORD freq_mhz;
	DWORD freq_mhz_len = sizeof(freq_mhz);
	if(RegQueryValueExA(hkey, "~MHz", nullptr, nullptr, static_cast<LPBYTE>(static_cast<void*>(&freq_mhz)), &freq_mhz_len))
		return {};

	return freq_mhz * 1'000'000;
}


#endif
