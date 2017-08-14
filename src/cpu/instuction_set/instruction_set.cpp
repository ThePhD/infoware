// infoware - C++ System information Library
//
// Written in 2017 by nabijaczleweli <nabijaczleweli@gmail.com> and ThePhD <phdofthehouse@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>

#include "infoware/cpu.hpp"
#include "infoware/detail/cpuid.hpp"

#if defined _WIN32 && defined _MSC_VER
#include <intrin.h>
#define INFOWARE_XCR_XFEATURE_ENABLED_MASK _XCR_XFEATURE_ENABLED_MASK
#else
#define INFOWARE_XCR_XFEATURE_ENABLED_MASK 0
#endif // Windows or not

static bool has_os_avx() {
	bool avxsupport = false;

	int32_t cpuinfo[4];
	iware::detail::cpuid(cpuinfo, 1);

	bool osusesxsave_restore = (cpuinfo[2] & (1 << 27)) != 0;
	bool cpusupportsavx = (cpuinfo[2] & (1 << 28)) != 0;

	if (osusesxsave_restore && cpusupportsavx) {
		uint64_t xcrFeatureMask = iware::detail::xgetbv(INFOWARE_XCR_XFEATURE_ENABLED_MASK);
		avxsupport = (xcrFeatureMask & 0x6) == 0x6;
	}

	return avxsupport;
}

static bool has_os_avx_512() {
	if (!has_os_avx())
		return false;
	uint64_t xcrFeatureMask = iware::detail::xgetbv(INFOWARE_XCR_XFEATURE_ENABLED_MASK);
	return (xcrFeatureMask & 0xe6) == 0xe6;
}

std::vector<iware::cpu::instruction_set_t> iware::cpu::supported_instruction_sets() {
	int info[4];
	iware::detail::cpuid(info, 0);
	int idinfo = info[0];

	iware::detail::cpuid(info, 0x80000000);
	uint32_t extendedidinfo = info[0];
	std::vector<iware::cpu::instruction_set_t> supported;
	supported.reserve(64);

#define ADD_SET_IF( infoindex, bitindex, name ) if ((info[(infoindex)] & ((int)1 << (bitindex)) ) != 0) { supported.push_back(iware::cpu::name); }

	if (idinfo >= 0x00000001) {
		iware::detail::cpuid(info, 0x00000001);
		ADD_SET_IF(3, 22, instruction_set_t::mmx_extended);
		ADD_SET_IF(3, 23, instruction_set_t::mmx);
		ADD_SET_IF(3, 25, instruction_set_t::sse);
		ADD_SET_IF(3, 26, instruction_set_t::sse2);
		
		ADD_SET_IF(2, 0, instruction_set_t::sse3);
		ADD_SET_IF(2, 9, instruction_set_t::ssse3);
		ADD_SET_IF(2, 19, instruction_set_t::sse41);
		ADD_SET_IF(2, 20, instruction_set_t::sse42);
		ADD_SET_IF(2, 25, instruction_set_t::aes);
		ADD_SET_IF(2, 28, instruction_set_t::fma3);
		ADD_SET_IF(2, 12, instruction_set_t::rd_rand);
	}
	if (idinfo >= 0x00000007) {
		iware::detail::cpuid(info, 0x00000007);

		if (has_os_avx()) {
			// Both must be present to have AVX
			ADD_SET_IF(1, 5, instruction_set_t::avx2);
		}
		ADD_SET_IF(1, 4, instruction_set_t::hle);
		
		ADD_SET_IF(1, 3, instruction_set_t::bmi1);
		ADD_SET_IF(1, 8, instruction_set_t::bmi2);
		ADD_SET_IF(1, 19, instruction_set_t::adx);
		ADD_SET_IF(1, 14, instruction_set_t::mpx);
		ADD_SET_IF(1, 29, instruction_set_t::sha);

		if (has_os_avx_512()) {
			supported.push_back(iware::cpu::instruction_set_t::avx_512);
			ADD_SET_IF(1, 16, instruction_set_t::avx_512_f);
			ADD_SET_IF(1, 28, instruction_set_t::avx_512_cd);
			ADD_SET_IF(1, 26, instruction_set_t::avx_512_pf);
			ADD_SET_IF(1, 27, instruction_set_t::avx_512_er);
			ADD_SET_IF(1, 31, instruction_set_t::avx_512_vl);
			ADD_SET_IF(1, 30, instruction_set_t::avx_512_bw);
			ADD_SET_IF(1, 17, instruction_set_t::avx_512_dq);
			ADD_SET_IF(1, 21, instruction_set_t::avx_512_ifma);
			ADD_SET_IF(2, 1, instruction_set_t::avx_512_vbmi);
		}

		ADD_SET_IF(2, 0, instruction_set_t::prefetch_wt1);
	}
	if (extendedidinfo >= 0x80000001) {
		iware::detail::cpuid(info, 0x80000001);

		ADD_SET_IF(3, 29, instruction_set_t::x64);
		ADD_SET_IF(3, 0, instruction_set_t::x87_fpu);
		ADD_SET_IF(3, 30, instruction_set_t::s3d_now);
		ADD_SET_IF(3, 31, instruction_set_t::s3d_now_extended);
		ADD_SET_IF(3, 29, instruction_set_t::x64);
		ADD_SET_IF(2, 5, instruction_set_t::bmi1);
		ADD_SET_IF(2, 6, instruction_set_t::bmi2);
		ADD_SET_IF(2, 16, instruction_set_t::adx);
		ADD_SET_IF(2, 11, instruction_set_t::mpx);
	}
#undef ADD_SET_IF

	return supported;
}