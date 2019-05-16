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


#include "infoware/detail/sysctl.hpp"
#include <cstdint>
#include <cstring>
#include <sys/sysctl.h>
#include <sys/types.h>


// Requires data.size() >= sizeof(T)
template <class T>
static std::pair<bool, T> deconstruct_specific_int(const std::vector<char>& data) {
	std::pair<bool, T> out{true, {}};
	std::memcpy(&out.second, data.data(), sizeof(out.second));
	return out;
}


std::vector<char> iware::detail::sysctl(const char* name) {
	std::size_t len{};
	if(sysctlbyname(name, nullptr, &len, nullptr, 0))
		return {};

	std::vector<char> ret(len);
	if(sysctlbyname(name, ret.data(), &len, nullptr, 0))
		return {};

	return ret;
}

std::pair<bool, std::uint64_t> iware::detail::deconstruct_sysctl_int(const std::vector<char>& data) {
	switch(data.size()) {
		case sizeof(std::uint16_t):
			return deconstruct_specific_int<std::uint16_t>(data);

		case sizeof(std::uint32_t):
			return deconstruct_specific_int<std::uint32_t>(data);

		case sizeof(std::uint64_t):
			return deconstruct_specific_int<std::uint64_t>(data);

		default:
			return {};
	}
}

#endif
