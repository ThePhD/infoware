// SPDX-License-Identifier: CC0-1.0
// infoware - C++ System information Library


#ifndef _WIN32
#ifndef __APPLE__


#include "infoware/cpu.hpp"
#include <fstream>
#include <string>


static std::string cpuinfo_value(const char* key) {
	std::ifstream cpuinfo("/proc/cpuinfo");

	if(!cpuinfo.is_open() || !cpuinfo)
		return {};

	for(std::string line; std::getline(cpuinfo, line);)
		if(line.find(key) == 0) {
			const auto colon_id    = line.find_first_of(':');
			const auto nonspace_id = line.find_first_not_of(" \t", colon_id + 1);
			return line.c_str() + nonspace_id;
		}

	return {};
}


std::string iware::cpu::vendor() {
	return cpuinfo_value("vendor");
}

std::string iware::cpu::model_name() {
	return cpuinfo_value("model name");
}


#endif
#endif
