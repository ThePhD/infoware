// infoware - C++ System information Library
//
// Written in 2017 by nabijaczleweli <nabijaczleweli@gmail.com> and ThePhD <phdofthehouse@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>


#ifndef _WIN32


#include "infoware/system.hpp"
#include <cstdlib>
#include <fstream>
#include <sys/utsname.h>


iware::system::OS_info_t iware::system::OS_info() {
	std::ifstream release("/etc/lsb-release");

	if(!release.is_open() || !release)
		return {};

	iware::system::OS_info_t ret{};

	for(std::string line; std::getline(release, line);)
		if(line.find("DISTRIB_ID") == 0)
			ret.name = line.substr(line.find('=') + 1);
		else if(line.find("DISTRIB_RELEASE") == 0) {
			char* marker     = &line[line.find('=') + 1];
			ret.major        = std::strtoul(marker, &marker, 10);
			ret.minor        = std::strtoul(marker + 1, &marker, 10);
			ret.patch        = std::strtoul(marker + 1, &marker, 10);
			ret.build_number = std::strtoul(marker + 1, nullptr, 10);
		} else if(line.find("DISTRIB_DESCRIPTION") == 0) {
			const auto start_idx = line.find('"') + 1;
			const auto end_idx   = line.size() - 1;
			ret.full_name        = line.substr(start_idx, end_idx - start_idx);
		}

	return ret;
}


#endif
