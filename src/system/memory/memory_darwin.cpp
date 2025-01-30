// SPDX-License-Identifier: CC0-1.0
// infoware - C++ System information Library


#ifdef __APPLE__


#include "infoware/detail/sysctl.hpp"
#include "infoware/system.hpp"
#include <cstdint>
#include <mach/mach.h>
#include <mach/vm_statistics.h>
#include <sys/sysctl.h>
#include <sys/types.h>
#include <unistd.h>


/// Adapted from https://stackoverflow.com/q/14789672/2851815
iware::system::memory_t iware::system::memory() noexcept {
	iware::system::memory_t ret{};

	const auto ctl_ram = iware::detail::sysctl(CTL_HW, HW_MEMSIZE);
	if(!ctl_ram.empty()) {
		const auto ram = iware::detail::deconstruct_sysctl_int(ctl_ram);
		if(ram.first)
			ret.virtual_total = ram.second;
	}

	std::uint32_t mem_free;
	auto mem_free_length = sizeof(mem_free);
	if(sysctlbyname("kern.memorystatus_level", &mem_free, &mem_free_length, nullptr, 0) != -1)
		ret.virtual_available = mem_free * ret.virtual_total / 100;
	else {
		const auto host = mach_host_self();

		vm_statistics64 stats;
		natural_t count = HOST_VM_INFO64_COUNT;
		if(host_statistics64(host, HOST_VM_INFO64, reinterpret_cast<host_info64_t>(&stats), &count) == KERN_SUCCESS)
			ret.virtual_available = stats.free_count * getpagesize();
	}

	ret.physical_total     = ret.virtual_total;
	ret.physical_available = ret.virtual_available;

	return ret;
}


#endif
