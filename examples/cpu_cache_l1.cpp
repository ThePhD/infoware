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
	const auto cache = iware::cpu::cache(1);
	std::cout << std::boolalpha << "L1 cache: size " << cache.size << ", line size: " << cache.line_size
	          << ", associativity: " << static_cast<unsigned int>(cache.associativity) << ", type: ";
	switch(cache.type) {
		case iware::cpu::cache_type_t::unified:
			std::cout << "unified";
			break;
		case iware::cpu::cache_type_t::instruction:
			std::cout << "instruction";
			break;
		case iware::cpu::cache_type_t::data:
			std::cout << "data";
			break;
		case iware::cpu::cache_type_t::trace:
			std::cout << "trace";
			break;
	}
	std::cout << '\n';
}
