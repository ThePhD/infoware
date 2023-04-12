// SPDX-License-Identifier: CC0-1.0
// infoware - C++ System information Library


#ifndef _WIN32
#ifndef __APPLE__


#include "infoware/system.hpp"
#include <fstream>
#include <string>


iware::system::memory_t iware::system::memory() noexcept {
	std::ifstream meminfo("/proc/meminfo");

	if(!meminfo.is_open() || !meminfo)
		return {};

	iware::system::memory_t ret;
	for(std::string line; std::getline(meminfo, line);) {
		const auto colon_id = line.find_first_of(':');
		const auto value    = std::strtoul(line.c_str() + colon_id + 1, nullptr, 10) * 1024;

		if(line.find("MemTotal") == 0)
			ret.physical_total = value;
		else if(line.find("MemAvailable") == 0)
			ret.physical_available = value;
		else if(line.find("VmallocTotal") == 0)
			ret.virtual_total = value;
		else if(line.find("VmallocUsed") == 0)
			ret.virtual_available = ret.virtual_total - value;
	}

	return ret;
}


#endif
#endif
