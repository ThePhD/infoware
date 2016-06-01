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
	switch(iware::cpu::endianness()) {
		case iware::cpu::endianness_t::little:
			std::cout << "Little-Endian";
			break;
		case iware::cpu::endianness_t::big:
			std::cout << "Big-Endian";
			break;
	}

	std::cout << '\n';
}
