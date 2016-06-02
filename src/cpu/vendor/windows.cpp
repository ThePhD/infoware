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


std::string iware::cpu::vendor() {
	HKEY hkey;
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, R"(HARDWARE\DESCRIPTION\System\CentralProcessor\0)", 0, KEY_READ, &hkey))
		return {};

	char identifier[13];
	unsigned long identifier_len = sizeof identifier;
	if(RegQueryValueEx(hkey, "VendorIdentifier", nullptr, nullptr, static_cast<unsigned char*>(static_cast<void*>(identifier)), &identifier_len))
		return {};

	return identifier;
}


#endif
