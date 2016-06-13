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


#pragma once


namespace iware {

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) || defined(_WIN32_) || defined(__WIN32__)

#define INFOWARE_WIN 1

#if defined(_WIN64)
#define INFOWARE_X64 1
#define INFOWARE_WIN64 1
#else
#define INFOWARE_X86 1
#define INFOWARE_WIN32 1
#endif // WIN64 

#elif defined(__APPLE__) || defined(_APPLE) || defined(APPLE) || defined(MACOS) || defined(MACOSX) || defined(_MACOS_) || defined(_MACOSX_)

#define INFOWARE_APPLE 1

// TODO: what are the 64-bit defines we can rely on for Apple?

#define INFOWARE_APPLE32 1
#define INFOWARE_X86 1

#elif defined(__LINUX__) || defined( _UNIX ) || defined( _POSIX )

#define INFOWARE_LINUX 1

#define INFOWARE_LINUX32 1
#define INFOWARE_X86 1

#else /* Or something else */

#error Unknown Platform - please define the platform in PlatformOS.h

#endif // WIN32 || APPLE || LINUX 

}
