// infoware - C++ System information Library
//
// Written in 2016-2020 by nabijaczleweli <nabijaczleweli@gmail.com> and ThePhD <phdofthehouse@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>


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
