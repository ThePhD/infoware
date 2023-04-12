// SPDX-License-Identifier: CC0-1.0
// infoware - C++ System information Library


#include "infoware/cpu.hpp"
#include "infoware/detail/sysctl.hpp"
#include "infoware/platform.hpp"
#include <cstring>
#include <string>


#if __APPLE__ && INFOWARE_ARM
#include <sys/sysctl.h>

std::string iware::cpu::vendor_id() {
	return iware::detail::sysctl(CTL_KERN, HW_MACHINE);
}
#endif /* INFOWARE_ARM */
