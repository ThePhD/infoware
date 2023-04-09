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

#pragma once

#if defined(__i386__) || defined(__x86_64__) || defined (_M_X64) || defined (_M_IX86)
#define INFOWARE_X86_FAMILY 1
#endif /* x86 family */

#if defined (__arm64__) || defined (_M_ARM64)
#define INFOWARE_ARM 1
#endif /* arm */

#if __APPLE__ || __MACH__
#define INFOWARE_APPLE 1
#endif /* apple */
