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


#include "infoware/cpu.hpp"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <utility>


static std::pair<const char* const*, const char* const*> instruction_set_to_names(iware::cpu::instruction_set_t set) {
	static const char* mmx[]       = {"mmx"};
	static const char* threednow[] = {"3dnow", "3dnowext"};
	static const char* sse[]       = {"sse"};
	static const char* sse2[]      = {"sse2"};
	static const char* sse3[]      = {"ssse3", "sse3"};
	static const char* avx[]       = {"avx"};

	switch(set) {
		case iware::cpu::instruction_set_t::s3d_now:
			return {threednow, threednow + sizeof threednow / sizeof(*threednow)};
		case iware::cpu::instruction_set_t::mmx:
			return {mmx, mmx + sizeof mmx / sizeof(*mmx)};
		case iware::cpu::instruction_set_t::sse:
			return {sse, sse + sizeof sse / sizeof(*sse)};
		case iware::cpu::instruction_set_t::sse2:
			return {sse2, sse2 + sizeof sse2 / sizeof(*sse2)};
		case iware::cpu::instruction_set_t::sse3:
			return {sse3, sse3 + sizeof sse3 / sizeof(*sse3)};
		case iware::cpu::instruction_set_t::avx:
			return {avx, avx + sizeof avx / sizeof(*avx)};
		default:
			return {nullptr, nullptr};
	}
}


bool iware::cpu::instruction_set_supported(iware::cpu::instruction_set_t set) {
	std::vector<iware::cpu::instruction_set_t> ises = supported_instruction_sets();
	if (std::find(ises.cbegin(), ises.cend(), set) != ises.cend()) {
		return true;
	}

	const auto set_names = instruction_set_to_names(set);
	if(!set_names.first)
		return false;

	std::ifstream cpuinfo("/proc/cpuinfo");

	if(!cpuinfo.is_open() || !cpuinfo)
		return false;

	for(std::string tmp; std::getline(cpuinfo, tmp);)
		if(tmp.find("flags") == 0) {
			const auto colon_id = tmp.find_first_of(':');
			std::istringstream is(tmp.c_str() + colon_id + 1);
			while(is >> tmp)
				if(std::any_of(set_names.first, set_names.second, [&](auto name) { return tmp == name; }))
					return true;
		}

	return false;
}


#endif
