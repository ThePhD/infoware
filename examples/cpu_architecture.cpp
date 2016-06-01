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


#include "infoware/cpu.hpp"
#include <iostream>


int main() {
	switch(iware::cpu::architecture()) {
		case iware::cpu::architecture_t::x64:
			std::cout << "x64";
			break;
		case iware::cpu::architecture_t::ARM:
			std::cout << "ARM";
			break;
		case iware::cpu::architecture_t::itanium:
			std::cout << "IA64";
			break;
		case iware::cpu::architecture_t::x86:
			std::cout << "x86";
			break;
		case iware::cpu::architecture_t::unknown:
			std::cout << "Unknown architecture";
			break;
	}

	std::cout << '\n';
}
