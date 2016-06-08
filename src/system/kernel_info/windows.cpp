// infoware - C++ System information Library
//
// Written in 2016 by nabijaczleweli <nabijaczleweli@gmail.com> and ThePhD <phdofthehouse@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>


#ifdef _WIN32


#include "infoware/system.hpp"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>


// https://msdn.microsoft.com/en-us/library/windows/desktop/ms724439(v=vs.85).aspx
iware::system::kernel_info_t iware::system::kernel_info() noexcept {
	const auto version = GetVersion();

	unsigned int major_version = LOBYTE(LOWORD(version));
	unsigned int minor_version = HIBYTE(LOWORD(version));

	unsigned int build{};
	if(version < 0x80000000)
		build = HIWORD(version);

	return {iware::system::kernel_t::windows_nt, major_version, minor_version, 0, build};
}


#endif
