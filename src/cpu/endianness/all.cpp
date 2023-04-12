// SPDX-License-Identifier: CC0-1.0
// infoware - C++ System information Library


#include "infoware/cpu.hpp"
#include <cstdint>


iware::cpu::endianness_t iware::cpu::endianness() noexcept {
	const std::uint16_t test = 0xFF00;
	const auto result        = *static_cast<const std::uint8_t*>(static_cast<const void*>(&test));

	if(result == 0xFF)
		return iware::cpu::endianness_t::big;
	else
		return iware::cpu::endianness_t::little;
}
