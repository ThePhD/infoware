// SPDX-License-Identifier: CC0-1.0
// infoware - C++ System information Library


#ifdef _WIN32


#include "infoware/system.hpp"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>


iware::system::memory_t iware::system::memory() noexcept {
	MEMORYSTATUSEX mem;
	mem.dwLength = sizeof(mem);
	if(!GlobalMemoryStatusEx(&mem))
		return {};

	return {mem.ullAvailPhys, mem.ullTotalPhys, mem.ullAvailVirtual, mem.ullTotalVirtual};
}


#endif
