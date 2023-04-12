// SPDX-License-Identifier: CC0-1.0
// infoware - C++ System information Library


#include "infoware/platform.hpp"
#if __APPLE__ && INFOWARE_ARM


#include "infoware/cpu.hpp"
#include <sys/sysctl.h>
#include <unistd.h>


static bool has_feature(const char* feature) {
	int value        = 0;
	std::size_t size = sizeof(value);
	return !sysctlbyname(feature, &value, &size, nullptr, 0) && value;
}

static bool feature_is_supported(const char* name) {
	return has_feature(name);
}

template <typename... Names>
static bool feature_is_supported(const char* name, Names... names) {
	return has_feature(name) || feature_is_supported(names...);
}

template <typename... Names>
static void push_if_supported(std::vector<iware::cpu::instruction_set_t>& instructions, iware::cpu::instruction_set_t instruction, Names... names) {
	if(feature_is_supported(names...)) {
		instructions.push_back(instruction);
	}
}


std::vector<iware::cpu::instruction_set_t> iware::cpu::supported_instruction_sets() {
	std::vector<instruction_set_t> instructions;

	push_if_supported(instructions, iware::cpu::instruction_set_t::sha, "hw.optional.arm.FEAT_SHA3", "hw.optional.arm.FEAT_SHA256", "hw.optional.arm.FEAT_SHA512",
	                  "hw.optional.arm.FEAT_SHA1", "hw.optional.armv8_2_sha512", "hw.optional.armv8_2_sha3");
	push_if_supported(instructions, iware::cpu::instruction_set_t::aes, "hw.optional.arm.FEAT_AES");

	push_if_supported(instructions, iware::cpu::instruction_set_t::fhm, "hw.optional.arm.FEAT_FHM");
	push_if_supported(instructions, iware::cpu::instruction_set_t::dotprod, "hw.optional.arm.FEAT_DotProd");
	push_if_supported(instructions, iware::cpu::instruction_set_t::rdm, "hw.optional.arm.FEAT_RDM");
	push_if_supported(instructions, iware::cpu::instruction_set_t::lse, "hw.optional.arm.FEAT_LSE");
	push_if_supported(instructions, iware::cpu::instruction_set_t::pmnull, "hw.optional.arm.FEAT_PMNULL");
	push_if_supported(instructions, iware::cpu::instruction_set_t::specres, "hw.optional.arm.FEAT_SPECRES");
	push_if_supported(instructions, iware::cpu::instruction_set_t::sb, "hw.optional.arm.FEAT_SB");
	push_if_supported(instructions, iware::cpu::instruction_set_t::frintts, "hw.optional.arm.FEAT_FRINTTS");
	push_if_supported(instructions, iware::cpu::instruction_set_t::lrcpc, "hw.optional.arm.FEAT_LRCPC");
	push_if_supported(instructions, iware::cpu::instruction_set_t::lrcpc2, "hw.optional.arm.FEAT_LRCPC2");
	push_if_supported(instructions, iware::cpu::instruction_set_t::fcma, "hw.optional.arm.FEAT_FCMA");
	push_if_supported(instructions, iware::cpu::instruction_set_t::jscvt, "hw.optional.arm.FEAT_JSCVT");
	push_if_supported(instructions, iware::cpu::instruction_set_t::pauth, "hw.optional.arm.FEAT_PAuth");
	push_if_supported(instructions, iware::cpu::instruction_set_t::pauth2, "hw.optional.arm.FEAT_PAuth2");
	push_if_supported(instructions, iware::cpu::instruction_set_t::fpac, "hw.optional.arm.FEAT_FPAC");
	push_if_supported(instructions, iware::cpu::instruction_set_t::dpb, "hw.optional.arm.FEAT_DPB");
	push_if_supported(instructions, iware::cpu::instruction_set_t::dpb2, "hw.optional.arm.FEAT_DPB2");
	push_if_supported(instructions, iware::cpu::instruction_set_t::bf16, "hw.optional.arm.FEAT_BF16");
	push_if_supported(instructions, iware::cpu::instruction_set_t::i8mm, "hw.optional.arm.FEAT_BF16");
	push_if_supported(instructions, iware::cpu::instruction_set_t::ecv, "hw.optional.arm.FEAT_ECV");
	push_if_supported(instructions, iware::cpu::instruction_set_t::les2, "hw.optional.arm.FEAT_LES2");
	push_if_supported(instructions, iware::cpu::instruction_set_t::csv2, "hw.optional.arm.FEAT_CSV2");
	push_if_supported(instructions, iware::cpu::instruction_set_t::csv3, "hw.optional.arm.FEAT_CSV3");
	push_if_supported(instructions, iware::cpu::instruction_set_t::dit, "hw.optional.arm.FEAT_DIT");
	push_if_supported(instructions, iware::cpu::instruction_set_t::fp16, "hw.optional.arm.FEAT_FP16", "hw.optional.neon_fp16");
	push_if_supported(instructions, iware::cpu::instruction_set_t::ssbs, "hw.optional.arm.FEAT_SSBS");
	push_if_supported(instructions, iware::cpu::instruction_set_t::bti, "hw.optional.arm.FEAT_BTI");
	push_if_supported(instructions, iware::cpu::instruction_set_t::fp_sync_exception, "hw.optional.arm.FP_SyncExceptions");
	push_if_supported(instructions, iware::cpu::instruction_set_t::neon, "hw.optional.neon");
	push_if_supported(instructions, iware::cpu::instruction_set_t::armv8_1_atomics, "hw.optional.armv8_1_atomics");
	push_if_supported(instructions, iware::cpu::instruction_set_t::armv8_2_fhm, "hw.optional.armv8_2_fhm");
	push_if_supported(instructions, iware::cpu::instruction_set_t::armv8_2_compnum, "hw.optional.armv8_2_compnum");
	push_if_supported(instructions, iware::cpu::instruction_set_t::watchpoint, "hw.optional.watchpoint");
	push_if_supported(instructions, iware::cpu::instruction_set_t::breakpoint, "hw.optional.breakpoint");
	push_if_supported(instructions, iware::cpu::instruction_set_t::armv8_crc32, "hw.optional.armv8_crc32");
	push_if_supported(instructions, iware::cpu::instruction_set_t::armv8_gpi, "hw.optional.armv8_gpi");
	push_if_supported(instructions, iware::cpu::instruction_set_t::adv_simd, "hw.optional.AdvSIMD");
	push_if_supported(instructions, iware::cpu::instruction_set_t::adv_simd_hpfp_cvt, "hw.optional.AdvSIMD_HPFPCvt");
	push_if_supported(instructions, iware::cpu::instruction_set_t::ucnormal_mem, "hw.optional.ucnormal_mem");
	return instructions;
}


#endif /* __APPLE__ && INFOWARE_ARM */
