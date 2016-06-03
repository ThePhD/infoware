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


#ifdef _WIN32


#include "infoware/cpu.hpp"
#include <bitset>
#include <vector>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>


// C++ified http://stackoverflow.com/a/28894219/2851815
iware::cpu::quantities_t iware::cpu::quantities() {
	std::vector<SYSTEM_LOGICAL_PROCESSOR_INFORMATION> buffer;
	{
		DWORD byte_count = 0;
		GetLogicalProcessorInformation(nullptr, &byte_count);
		buffer.resize(byte_count / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION));
		GetLogicalProcessorInformation(buffer.data(), &byte_count);
	}

	iware::cpu::quantities_t ret{};
	for(auto&& info : buffer)
		switch(info.Relationship) {
			case RelationProcessorCore:
				++ret.physical;
				// A hyperthreaded core supplies more than one logical processor.
				ret.logical += std::bitset<sizeof(ULONG_PTR) * 8>(info.ProcessorMask).count();
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


#endif
