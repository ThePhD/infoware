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
#include <cstring>
#include <sstream>
#include <stdio.h>
#include <string>
#include <unistd.h>


iware::cpu::quantities_t iware::cpu::quantities() {
	iware::cpu::quantities_t ret{};
	ret.logical = sysconf(_SC_NPROCESSORS_ONLN);

	// TODO rest of fields

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

	cache_t ret{};

	bool full_line = false;
	std::string line;
	char buf[32]{};
	for(bool have_data = true; have_data;) {
		have_data = fgets(buf, sizeof(buf), sysctl_output);

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
