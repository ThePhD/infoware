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


#ifdef __APPLE__


#include "infoware/cpu.hpp"
#include "infoware/detail/scope.hpp"
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <stdio.h>
#include <string>


// https://github.com/ThePhD/infoware/issues/12#issuecomment-495291650
//
// Assuming there's only ever gonna be 1 package because the sysctl outputs I've seen so far haven't provided any way to read if there were more?
//
// Parses `machdep.cpu.core_count` and `machdep.cpu.thread_count`.
iware::cpu::quantities_t iware::cpu::quantities() {
	const auto sysctl_output = popen("sysctl machdep.cpu", "r");
	if(!sysctl_output)
		return {};
	iware::detail::quickscope_wrapper sysctl_closer{[&]() { pclose(sysctl_output); }};

	iware::cpu::quantities_t ret{};

	char buf[48]{};
	while(fgets(buf, sizeof(buf), sysctl_output)) {
		const auto len = std::strlen(buf);
		if(len < 24 + 2 || buf[len - 1] != '\n')
			continue;  // Skipping all lines that don't fit in the buffer because the ones we're after do and the ones shorter than the ones we're after

		if(std::strncmp(buf + 12, "core_count: ", 22 - 12) == 0)
			ret.physical = std::strtoul(buf + 22 + 2, nullptr, 10);
		else if(std::strncmp(buf + 12, "thread_count: ", 24 - 12) == 0)
			ret.logical = std::strtoul(buf + 24 + 2, nullptr, 10);
	}

	ret.packages = 1;
	return ret;
}

// This is hell
//
// https://github.com/ThePhD/infoware/issues/12#issuecomment-495782115
//
// TODO: couldn't find a good way to get the associativity (default 0) or the type (default unified)
iware::cpu::cache_t iware::cpu::cache(unsigned int level) {
	const auto sysctl_output = popen("sysctl hw", "r");
	if(!sysctl_output)
		return {};
	iware::detail::quickscope_wrapper sysctl_closer{[&]() { pclose(sysctl_output); }};

	iware::cpu::cache_t ret{};

	bool full_line = false;
	std::string line;
	char buf[32]{};
	while(fgets(buf, sizeof(buf), sysctl_output)) {
		if(full_line)
			line = buf;
		else
			line += buf;

		full_line = line.back() == '\n';
		if(full_line) {
			std::size_t* data = nullptr;
			if(line.find("hw.cachelinesize") == 0)
				data = &ret.line_size;
			else if(line.find("hw.l") == 0 && line.find(std::to_string(level)) == 4 && [&]() {
				        const auto idx = line.find("cachesize");
				        return idx == 5 ||  // e.g. hw.l2cachesize
				               idx == 6;    // e.g. hw.l1icachesize
			        }())
				data = &ret.size;

			if(data) {
				std::size_t tmp;
				std::istringstream(line.c_str() + line.find(':') + 1) >> tmp;
				*data += tmp;
			}
		}
	}

	return ret;
}


#endif
