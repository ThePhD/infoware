// SPDX-License-Identifier: CC0-1.0
// infoware - C++ System information Library


#ifdef __APPLE__


#include "infoware/cpu.hpp"
#include "infoware/detail/sysctl.hpp"
#include <algorithm>
#include <cstring>
#include <utility>


static std::pair<const char* const*, const char* const*> instruction_set_to_names(iware::cpu::instruction_set_t set) {
	static const char* const mmx[]               = {"MMX"};
	static const char* const threednow[]         = {"3DNOW", "3DNOWEXT"};
	static const char* const sse[]               = {"SSE"};
	static const char* const sse2[]              = {"SSE2"};
	static const char* const sse3[]              = {"SSSE3", "SSE3"};
	static const char* const avx[]               = {"AVX"};
	static const char* const fhm[]               = {"FHM"};
	static const char* const dotprod[]           = {"DOTPROD"};
	static const char* const rdm[]               = {"RDM"};
	static const char* const lse[]               = {"LSE"};
	static const char* const pmnull[]            = {"PMNULL"};
	static const char* const specres[]           = {"SPECRES"};
	static const char* const sb[]                = {"SB"};
	static const char* const frintts[]           = {"FRINTTS"};
	static const char* const lrcpc[]             = {"LRCPC"};
	static const char* const lrcpc2[]            = {"LRCPC2"};
	static const char* const fcma[]              = {"FCMA"};
	static const char* const jscvt[]             = {"JSCVT"};
	static const char* const pauth[]             = {"PAUTH"};
	static const char* const pauth2[]            = {"PAUTH2"};
	static const char* const fpac[]              = {"FPAC"};
	static const char* const dpb[]               = {"DPB"};
	static const char* const dpb2[]              = {"DPB2"};
	static const char* const bf16[]              = {"BF16"};
	static const char* const i8mm[]              = {"I8MM"};
	static const char* const ecv[]               = {"ECV"};
	static const char* const les2[]              = {"LES2"};
	static const char* const csv2[]              = {"CSV2"};
	static const char* const csv3[]              = {"CSV3"};
	static const char* const dit[]               = {"DIT"};
	static const char* const fp16[]              = {"FP16"};
	static const char* const ssbs[]              = {"SSBS"};
	static const char* const bti[]               = {"BTI"};
	static const char* const fp_sync_exception[] = {"FP_SYNC_EXCEPTION"};
	static const char* const neon[]              = {"NEON"};
	static const char* const armv8_1_atomics[]   = {"ARMV8_1_ATOMICS"};
	static const char* const armv8_2_fhm[]       = {"ARMV8_2_FHM"};
	static const char* const armv8_2_compnum[]   = {"ARMV8_2_COMPNUM"};
	static const char* const watchpoint[]        = {"WATCHPOINT"};
	static const char* const breakpoint[]        = {"BREAKPOINT"};
	static const char* const armv8_crc32[]       = {"ARMV8_CRC32"};
	static const char* const armv8_gpi[]         = {"ARMV8_GPI"};
	static const char* const adv_simd[]          = {"ADV_SIMD"};
	static const char* const adv_simd_hpfp_cvt[] = {"ADV_SIMD_HPFP_CVT"};
	static const char* const ucnormal_mem[]      = {"UCNORMAL_MEM"};


#define HANDLED_CASE(iset, variable)        \
	case iware::cpu::instruction_set_t::iset: \
		return {variable, variable + (sizeof(variable) / sizeof(*variable))};

	switch(set) {
		HANDLED_CASE(s3d_now, threednow)
		HANDLED_CASE(mmx, mmx)
		HANDLED_CASE(sse, sse)
		HANDLED_CASE(sse2, sse2)
		HANDLED_CASE(sse3, sse3)
		HANDLED_CASE(avx, avx)
		HANDLED_CASE(fhm, fhm)
		HANDLED_CASE(dotprod, dotprod)
		HANDLED_CASE(rdm, rdm)
		HANDLED_CASE(lse, lse)
		HANDLED_CASE(pmnull, pmnull)
		HANDLED_CASE(specres, specres)
		HANDLED_CASE(sb, sb)
		HANDLED_CASE(frintts, frintts)
		HANDLED_CASE(lrcpc, lrcpc)
		HANDLED_CASE(lrcpc2, lrcpc2)
		HANDLED_CASE(fcma, fcma)
		HANDLED_CASE(jscvt, jscvt)
		HANDLED_CASE(pauth, pauth)
		HANDLED_CASE(pauth2, pauth2)
		HANDLED_CASE(fpac, fpac)
		HANDLED_CASE(dpb, dpb)
		HANDLED_CASE(dpb2, dpb2)
		HANDLED_CASE(bf16, bf16)
		HANDLED_CASE(i8mm, i8mm)
		HANDLED_CASE(ecv, ecv)
		HANDLED_CASE(les2, les2)
		HANDLED_CASE(csv2, csv2)
		HANDLED_CASE(csv3, csv3)
		HANDLED_CASE(dit, dit)
		HANDLED_CASE(fp16, fp16)
		HANDLED_CASE(ssbs, ssbs)
		HANDLED_CASE(bti, bti)
		HANDLED_CASE(fp_sync_exception, fp_sync_exception)
		HANDLED_CASE(neon, neon)
		HANDLED_CASE(armv8_1_atomics, armv8_1_atomics)
		HANDLED_CASE(armv8_2_fhm, armv8_2_fhm)
		HANDLED_CASE(armv8_2_compnum, armv8_2_compnum)
		HANDLED_CASE(watchpoint, watchpoint)
		HANDLED_CASE(breakpoint, breakpoint)
		HANDLED_CASE(armv8_crc32, armv8_crc32)
		HANDLED_CASE(armv8_gpi, armv8_gpi)
		HANDLED_CASE(adv_simd, adv_simd)
		HANDLED_CASE(adv_simd_hpfp_cvt, adv_simd_hpfp_cvt)
		HANDLED_CASE(ucnormal_mem, ucnormal_mem)

		default:
			return {nullptr, nullptr};
	}

#undef HANDLED_CASE
}


bool iware::cpu::instruction_set_supported(iware::cpu::instruction_set_t set) {
	const auto ises = supported_instruction_sets();
	if(std::find(ises.begin(), ises.end(), set) != ises.cend())
		return true;

	const auto set_names = instruction_set_to_names(set);
	if(!set_names.first)
		return false;

	auto ctl_data = iware::detail::sysctl("machdep.cpu.features");
	if(ctl_data.empty())
		return 0;

	for(auto cur_name = std::strtok(&ctl_data[0], " \t\n"); cur_name; cur_name = std::strtok(nullptr, " \t\n"))
		if(std::any_of(set_names.first, set_names.second, [&](auto name) { return name == cur_name; }))
			return true;

	return false;
}


#endif
