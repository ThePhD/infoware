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
#include <stdio.h>


std::int64_t iware::cpu::frequency() noexcept {
	const auto sysctl_output = popen("sysctl hw.cpufrequency", "r");
	if(!sysctl_output)
		return 0;
	iware::detail::quickscope_wrapper sysctl_closer{[&]() { pclose(sysctl_output); }};

	char buf[64]{};  // We're expecting somewhere on the order of strlen("hw.cpufrequency: 3100000000") == 27
	if(!fgets(buf, sizeof(buf), sysctl_output))
		return 0;

	const char* colon = std::strchr(buf, ':');
	if(!colon)
		return 0;

	return static_cast<std::int64_t>(std::strtoull(colon + 1, nullptr, 10));
}


#endif
