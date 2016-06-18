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
#include "infoware/gpu.hpp"
#include "infoware/system.hpp"
#include <iostream>


int main() {
	iware::cpu::quantities_t quan = iware::cpu::quantities();
	std::cout << "==============\n"
	          << "CPU Properties\n"
	          << "==============\n"
	          << "\n"
	          << iware::cpu::vendor() << " - " << iware::cpu::model_name() << "\n"
	          << quan.packages << " packages with\n"
	          << "\t" << quan.logical << " logical cores, " << quan.physical << " physical cores total available\n";
	for(std::size_t i = 0; i < quan.packages; ++i) {
		// TODO: link package index into the various functions? (with a default of = 0)
		std::cout << "CPU Package " << i << ":"
		          << "\n";
		std::cout << "\t";
		switch(iware::cpu::architecture()) {
			case iware::cpu::architecture_t::x64:
				std::cout << "x64";
				break;
			case iware::cpu::architecture_t::ARM:
				std::cout << "ARM";
				break;
			case iware::cpu::architecture_t::itanium:
				std::cout << "IA64";
				break;
			case iware::cpu::architecture_t::x86:
				std::cout << "x86";
				break;
			case iware::cpu::architecture_t::unknown:
				std::cout << "Unknown architecture";
				break;
		}
		std::cout << ", ";
		switch(iware::cpu::endianness()) {
			case iware::cpu::endianness_t::little:
				std::cout << "Little-Endian";
				break;
			case iware::cpu::endianness_t::big:
				std::cout << "Big-Endian";
				break;
		}

		// TODO: Need to have logical core / physical core count on a PER-PACKAGE basis

		std::cout << "\n";
		std::cout << "\t" << iware::cpu::frequency() << " Hz Clock" << '\n';
		std::cout << "\tInstruction Sets\n"
		          << "\t\t3D Now! - " << iware::cpu::instruction_set_supported(iware::cpu::instruction_set_t::ThreeD_now) << '\n'
		          << "\t\tSSE - " << iware::cpu::instruction_set_supported(iware::cpu::instruction_set_t::SSE) << '\n'
		          << "\t\tSSE2 - " << iware::cpu::instruction_set_supported(iware::cpu::instruction_set_t::SSE2) << '\n'
		          << "\t\tSSE3 - " << iware::cpu::instruction_set_supported(iware::cpu::instruction_set_t::SSE3) << '\n'
		          << "\t\tAVX - " << iware::cpu::instruction_set_supported(iware::cpu::instruction_set_t::AVX) << '\n';

		iware::system::memory_t mem = iware::system::memory();
		std::cout << "\tMemory:\n"
		          << "\t\t" << mem.physical_available << " physical bytes available\n"
		          << "\t\t" << mem.physical_total << " physical bytes total\n"
		          << "\t\t" << mem.virtual_available << " virtual bytes available\n"
		          << "\t\t" << mem.virtual_total << " virtual bytes total\n";
		// Cache levels start at 1
		// Explicit support for such goes to 4 with Linux calls
		// TODO: get the number of caches rather than fix them in place?
		// TODO: use CPUID to enumerate much larger cache architectures
		for(int i = 1; i < 4; ++i) {
			const auto cache = iware::cpu::cache(i);
			std::cout << std::boolalpha << "\t"
			          << "L" << i << " cache: size " << cache.size << ", line size: " << cache.line_size
			          << ", associativity: " << static_cast<unsigned int>(cache.associativity) << ", type: ";
			switch(cache.type) {
				case iware::cpu::cache_type_t::unified:
					std::cout << "unified";
					break;
				case iware::cpu::cache_type_t::instruction:
					std::cout << "instruction";
					break;
				case iware::cpu::cache_type_t::data:
					std::cout << "data";
					break;
				case iware::cpu::cache_type_t::trace:
					std::cout << "trace";
					break;
			}
			std::cout << '\n';
		}
	}
	std::cout << "\n\n";

	auto devices = iware::gpu::device_properties();
	std::cout << "==============\n"
	          << "GPU Properties\n"
	          << "==============\n"
	          << "\n";
	std::cout << devices.size() << " devices"
	          << "\n";
	for(std::size_t i = 0; i < devices.size(); ++i) {
		iware::gpu::device_properties_t& device = devices[i];
		std::cout << "Device " << i << "\n";
		std::cout << "\t" << device.name << '\n' << "\t";
		switch(device.vendor) {
			case iware::gpu::vendor_t::AMD:
				std::cout << "AMD";
				break;
			case iware::gpu::vendor_t::intel:
				std::cout << "Intel";
				break;
			case iware::gpu::vendor_t::NVidia:
				std::cout << "NVidia";
				break;
			case iware::gpu::vendor_t::qualcomm:
				std::cout << "Qualcomm";
				break;
			case iware::gpu::vendor_t::microsoft:
				std::cout << "Microsoft";
				break;
			case iware::gpu::vendor_t::unknown:
				std::cout << "Unknown";
				break;
		}
		std::cout << "\n"
		          << "\t" << device.memory_size << " bytes, " << device.cache_size << " bytes cache"
		          << "\n";
	}
	std::cout << '\n';

	// TODO: concatenate system information here as well
}
