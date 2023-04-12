// SPDX-License-Identifier: CC0-1.0
// infoware - C++ System information Library


#ifndef _WIN32
#ifndef __APPLE__


#include "infoware/cpu.hpp"
#include <fstream>
#include <string>


std::uint64_t iware::cpu::frequency() noexcept {
	std::ifstream cpuinfo("/proc/cpuinfo");

	if(!cpuinfo.is_open() || !cpuinfo)
		return 0;

	for(std::string line; std::getline(cpuinfo, line);)
		if(line.find("cpu MHz") == 0) {
			const auto colon_id = line.find_first_of(':');
			return static_cast<std::uint64_t>(std::strtod(line.c_str() + colon_id + 1, nullptr) * 1'000'000);
		}

	return 0;
}


#endif
#endif
