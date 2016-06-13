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


#include "infoware/cpu.hpp"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>


unsigned int iware::cpu::frequency() noexcept {
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	return static_cast<unsigned int>(freq.QuadPart * 1000);
}


#endif
