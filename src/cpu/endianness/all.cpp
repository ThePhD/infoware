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
#include <cstdint>
#include <stdexcept>
#include <string>


iware::cpu::endianness_t iware::cpu::endianness() noexcept {
	const std::uint16_t test = 0xFF00;
	const auto result        = *static_cast<const std::uint8_t*>(static_cast<const void*>(&test));

	switch(result) {
		case 0xFF:
			return iware::cpu::endianness_t::little;
		case 0x00:
			return iware::cpu::endianness_t::little;
		default:
			throw std::domain_error(std::to_string(static_cast<unsigned int>(result)) + " is neither 0xFF nor 0x00");
	}
}
