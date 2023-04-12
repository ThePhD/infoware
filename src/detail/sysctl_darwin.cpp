// SPDX-License-Identifier: CC0-1.0
// infoware - C++ System information Library


#ifdef __APPLE__


#include "infoware/detail/sysctl.hpp"
#include <cstdint>
#include <cstring>
#include <string>
#include <sys/sysctl.h>
#include <sys/types.h>


// Requires data.size() >= sizeof(T)
template <class T>
static std::pair<bool, T> deconstruct_specific_int(const std::string& data) {
	std::pair<bool, T> out{true, {}};
	std::memcpy(&out.second, data.data(), sizeof(out.second));
	return out;
}


#define SYSCTL_IMPL(fname, ...)                     \
	std::size_t len{};                                \
	if(fname(__VA_ARGS__, nullptr, &len, nullptr, 0)) \
		return {};                                      \
                                                    \
	std::string ret(len, '\0');                       \
	if(fname(__VA_ARGS__, &ret[0], &len, nullptr, 0)) \
		return {};                                      \
                                                    \
	return ret

std::string iware::detail::sysctl(const char* name) {
	SYSCTL_IMPL(::sysctlbyname, name);
}

std::string iware::detail::sysctl(int mib_0, int mib_1) {
	int name[2]{mib_0, mib_1};

	SYSCTL_IMPL(::sysctl, name, sizeof(name) / sizeof(*name));
}


std::pair<bool, std::uint64_t> iware::detail::deconstruct_sysctl_int(const std::string& data) {
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
