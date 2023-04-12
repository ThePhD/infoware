// SPDX-License-Identifier: CC0-1.0
// infoware - C++ System information Library


#ifdef __APPLE__


#include "infoware/cpu.hpp"
#include "infoware/detail/sysctl.hpp"


std::string iware::cpu::vendor() {
	return iware::detail::sysctl("machdep.cpu.vendor");
}

std::string iware::cpu::model_name() {
	return iware::detail::sysctl("machdep.cpu.brand_string");
}


#endif
