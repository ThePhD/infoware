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


bool iware::cpu::instruction_set_supported(iware::cpu::instruction_set_t set) {
	DWORD feature;
	switch(set) {
		case iware::cpu::instruction_set_t::ThreeD_now:
			feature = PF_3DNOW_INSTRUCTIONS_AVAILABLE;
			break;
		case iware::cpu::instruction_set_t::MMX:
			feature = PF_MMX_INSTRUCTIONS_AVAILABLE;
			break;
		case iware::cpu::instruction_set_t::SSE:
			feature = PF_XMMI_INSTRUCTIONS_AVAILABLE;
			break;
		case iware::cpu::instruction_set_t::SSE2:
			feature = PF_XMMI64_INSTRUCTIONS_AVAILABLE;
			break;
		case iware::cpu::instruction_set_t::SSE3:
			feature = PF_SSE3_INSTRUCTIONS_AVAILABLE;
			break;
	}

	return IsProcessorFeaturePresent(feature) != 0;
}


#endif
