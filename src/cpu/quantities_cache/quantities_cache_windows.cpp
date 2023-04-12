// SPDX-License-Identifier: CC0-1.0
// infoware - C++ System information Library


#ifdef _WIN32


#include "infoware/cpu.hpp"
#include <bitset>
#include <vector>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>


static std::vector<SYSTEM_LOGICAL_PROCESSOR_INFORMATION> cpuinfo_buffer() {
	std::vector<SYSTEM_LOGICAL_PROCESSOR_INFORMATION> buffer;

	DWORD byte_count = 0;
	GetLogicalProcessorInformation(nullptr, &byte_count);
	buffer.resize(byte_count / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION));
	GetLogicalProcessorInformation(buffer.data(), &byte_count);

	return buffer;
}


// C++ified http://stackoverflow.com/a/28894219/2851815
iware::cpu::quantities_t iware::cpu::quantities() {
	iware::cpu::quantities_t ret{};
	for(auto&& info : cpuinfo_buffer())
		switch(info.Relationship) {
			case RelationProcessorCore:
				++ret.physical;
				// A hyperthreaded core supplies more than one logical processor.
				ret.logical += static_cast<std::uint32_t>(std::bitset<sizeof(ULONG_PTR) * 8>(static_cast<std::uintptr_t>(info.ProcessorMask)).count());
				break;

			case RelationProcessorPackage:
				// Logical processors share a physical package.
				++ret.packages;
				break;

			default:
				break;
		}

	return ret;
}

iware::cpu::cache_t iware::cpu::cache(unsigned int level) {
	for(auto&& info : cpuinfo_buffer())
		if(info.Relationship == RelationCache)
			// Cache data is in ptr->Cache, one CACHE_DESCRIPTOR structure for each cache.
			if(info.Cache.Level == level) {
				iware::cpu::cache_type_t type{};
				switch(info.Cache.Type) {
					case CacheUnified:
						type = iware::cpu::cache_type_t::unified;
						break;
					case CacheInstruction:
						type = iware::cpu::cache_type_t::instruction;
						break;
					case CacheData:
						type = iware::cpu::cache_type_t::data;
						break;
					case CacheTrace:
						type = iware::cpu::cache_type_t::trace;
						break;
				}
				return {info.Cache.Size, info.Cache.LineSize, info.Cache.Associativity, type};
			}

	return {};
}


#endif
