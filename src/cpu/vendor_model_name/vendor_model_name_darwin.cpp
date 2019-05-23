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
#include <stdio.h>
#include <string>


using namespace std::literals;


static std::string sysctl_value(const char* subkey) {
	const auto sysctl_output = popen(("sysctl machdep.cpu."s + subkey).c_str(), "r");
	if(!sysctl_output)
		return 0;
	iware::detail::quickscope_wrapper sysctl_closer{[&]() { pclose(sysctl_output); }};

	std::string line;
	for(char buf[32]{}; (line.empty() || line.back() != '\n') && fgets(buf, sizeof(buf), sysctl_output);)
		line += buf;
	line.back() = '\0';

	const auto colon_id    = line.find_first_of(':');
	const auto nonspace_id = line.find_first_not_of(" \t", colon_id) + 1;
	return line.c_str() + nonspace_id;
}


std::string iware::cpu::vendor() {
	return sysctl_value("vendor");
}

std::string iware::cpu::model_name() {
	return sysctl_value("brand_string");
}


#endif
