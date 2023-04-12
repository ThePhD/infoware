// SPDX-License-Identifier: CC0-1.0
// infoware - C++ System information Library


#ifdef __APPLE__


#include "infoware/cpu.hpp"
#include "infoware/detail/sysctl.hpp"


std::uint64_t iware::cpu::frequency() noexcept {
	const auto ctl_data = iware::detail::sysctl("hw.cpufrequency");
	if(ctl_data.empty())
		return 0;

	const auto data = iware::detail::deconstruct_sysctl_int(ctl_data);
	if(!data.first)
		return 0;

	return data.second;
}


#endif
