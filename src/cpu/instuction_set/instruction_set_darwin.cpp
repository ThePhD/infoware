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
#include <algorithm>
#include <sstream>
#include <stdio.h>
#include <string>
#include <utility>


static std::pair<const char* const*, const char* const*> instruction_set_to_names(iware::cpu::instruction_set_t set) {
	static const char* mmx[]       = {"MMX"};
	static const char* threednow[] = {"3DNOW", "3DNOWEXT"};
	static const char* sse[]       = {"SSE"};
	static const char* sse2[]      = {"SSE2"};
	static const char* sse3[]      = {"SSSE3", "SSE3"};
	static const char* avx[]       = {"AVX"};


#define HANDLED_CASE(iset, variable)        \
	case iware::cpu::instruction_set_t::iset: \
		return {variable, variable + (sizeof(variable) / sizeof(*variable))};

	switch(set) {
		HANDLED_CASE(s3d_now, threednow)
		HANDLED_CASE(mmx, mmx)
		HANDLED_CASE(sse, sse)
		HANDLED_CASE(sse2, sse2)
		HANDLED_CASE(sse3, sse3)
		HANDLED_CASE(avx, avx)

		default:
			return {nullptr, nullptr};
	}

#undef HANDLED_CASE
}


bool iware::cpu::instruction_set_supported(iware::cpu::instruction_set_t set) {
	const auto ises = supported_instruction_sets();
	if(std::find(ises.begin(), ises.end(), set) != ises.cend())
		return true;

	const auto set_names = instruction_set_to_names(set);
	if(!set_names.first)
		return false;

	const auto sysctl_output = popen("sysctl machdep.cpu.features", "r");
	if(!sysctl_output)
		return false;
	iware::detail::quickscope_wrapper sysctl_closer{[&]() { pclose(sysctl_output); }};

	std::string line;
	for(char buf[32]{}; (line.empty() || line.back() != '\n') && fgets(buf, sizeof(buf), sysctl_output);)
		line += buf;

	const auto colon_id = line.find_first_of(':');
	std::istringstream is(line.c_str() + colon_id + 1);
	for(std::string tmp; is >> tmp;)
		if(std::any_of(set_names.first, set_names.second, [&](auto name) { return tmp == name; }))
			return true;

	return false;
}


#endif
