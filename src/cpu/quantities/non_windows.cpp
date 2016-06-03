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


#ifndef _WIN32


#include "infoware/cpu.hpp"
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <unistd.h>
#include <vector>


// http://stackoverflow.com/a/150971/2851815
// http://thedaneshproject.com/posts/how-to-find-the-number-of-physical-cpus-in-linux/
iware::cpu::quantities_t iware::cpu::quantities() {
	iware::cpu::quantities_t ret{};
	ret.logical = sysconf(_SC_NPROCESSORS_ONLN);

	std::ifstream cpuinfo("/proc/cpuinfo");

	if(!cpuinfo.is_open() || !cpuinfo)
		return ret;

	std::vector<unsigned int> package_ids;
	for(std::string line; std::getline(cpuinfo, line);)
		if(line.find("physical id") == 0)
			package_ids.emplace_back(std::strtoul(line.c_str() + line.find_first_of("1234567890"), nullptr, 10));
		else if(line.find("processor") == 0)
			++ret.physical;

	std::sort(package_ids.begin(), package_ids.end());
	package_ids.erase(std::unique(package_ids.begin(), package_ids.end()), package_ids.end());
	ret.packages = package_ids.size();

	return ret;
}


#endif
