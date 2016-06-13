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

#if  defined(_MSC_VER)
#define INFOWARE_VCXX 1
#elif __GNUC__
#define INFOWARE_GCC 1
#if defined(__MINGW32__) || defined(__MINGW64__)
#define INFOWARE_MINGW 1
#endif // MinGW
#elif __clang__
#define INFOWARE_CLANG 1
#else
#error Unknown Compiler -- set unique define for this compiler
#endif // VC++ || GCC || Clang

}
