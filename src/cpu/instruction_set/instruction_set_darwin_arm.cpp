#include "infoware/platform.hpp"
#include "infoware/cpu.hpp"
#include <vector>

#if __APPLE__ && INFOWARE_ARM
#include <unistd.h>
#include <sys/sysctl.h>

static bool has_feature(const char* feature) {
      int value = 0;
      size_t size;
      int ret = sysctlbyname(feature, &value, &size, nullptr, 0);
      return value && ret == 0;
}

static bool feature_is_supported(const char* name) {
      return has_feature(name);
}

template<typename... Names>
static bool feature_is_supported(const char* name, Names... names) {
      return has_feature(name) || feature_is_supported(names...);
}

template<typename... Names>
static void push_if_supported(std::vector<iware::cpu::instruction_set_t>& instructions, 
                              iware::cpu::instruction_set_t instruction, 
                              Names... names) {
      if (feature_is_supported(names...)) {
            instructions.push_back(instruction);
      }
}

std::vector<iware::cpu::instruction_set_t> iware::cpu::supported_instruction_sets() {
      using i = iware::cpu::instruction_set_t;
      std::vector<instruction_set_t> instructions{};
      
      push_if_supported(instructions, i::sha, 
            "hw.optional.arm.FEAT_SHA3", 
            "hw.optional.arm.FEAT_SHA256",
            "hw.optional.arm.FEAT_SHA512",
            "hw.optional.arm.FEAT_SHA1",
            "hw.optional.armv8_2_sha512",
            "hw.optional.armv8_2_sha3");
      push_if_supported(instructions, i::aes, 
            "hw.optional.arm.FEAT_AES");
      
      push_if_supported(instructions, i::fhm, 
            "hw.optional.arm.FEAT_FHM");
      push_if_supported(instructions, i::dotprod, 
            "hw.optional.arm.FEAT_DotProd");
      push_if_supported(instructions, i::rdm, 
            "hw.optional.arm.FEAT_RDM");
      push_if_supported(instructions, i::lse, 
            "hw.optional.arm.FEAT_LSE");
      push_if_supported(instructions, i::pmnull, 
            "hw.optional.arm.FEAT_PMNULL");
      push_if_supported(instructions, i::specres, 
            "hw.optional.arm.FEAT_SPECRES");
      push_if_supported(instructions, i::sb, 
            "hw.optional.arm.FEAT_SB");
      push_if_supported(instructions, i::frintts, 
            "hw.optional.arm.FEAT_FRINTTS");
      push_if_supported(instructions, i::lrcpc, 
            "hw.optional.arm.FEAT_LRCPC");
      push_if_supported(instructions, i::lrcpc2, 
            "hw.optional.arm.FEAT_LRCPC2");
      push_if_supported(instructions, i::fcma, 
            "hw.optional.arm.FEAT_FCMA");
      push_if_supported(instructions, i::jscvt, 
            "hw.optional.arm.FEAT_JSCVT");
      push_if_supported(instructions, i::pauth, 
            "hw.optional.arm.FEAT_PAuth");
      push_if_supported(instructions, i::pauth2, 
            "hw.optional.arm.FEAT_PAuth2");
      push_if_supported(instructions, i::fpac, 
            "hw.optional.arm.FEAT_FPAC");
      push_if_supported(instructions, i::dpb, 
            "hw.optional.arm.FEAT_DPB");
      push_if_supported(instructions, i::dpb2, 
            "hw.optional.arm.FEAT_DPB2");
      push_if_supported(instructions, i::bf16, 
            "hw.optional.arm.FEAT_BF16");
      push_if_supported(instructions, i::i8mm, 
            "hw.optional.arm.FEAT_BF16");
      push_if_supported(instructions, i::ecv, 
            "hw.optional.arm.FEAT_ECV");
      push_if_supported(instructions, i::les2, 
            "hw.optional.arm.FEAT_LES2");
      push_if_supported(instructions, i::csv2, 
            "hw.optional.arm.FEAT_CSV2");
      push_if_supported(instructions, i::csv3, 
            "hw.optional.arm.FEAT_CSV3");
      push_if_supported(instructions, i::dit, 
            "hw.optional.arm.FEAT_DIT");
      push_if_supported(instructions, i::fp16, 
            "hw.optional.arm.FEAT_FP16", "hw.optional.neon_fp16");
      push_if_supported(instructions, i::ssbs, 
            "hw.optional.arm.FEAT_SSBS");
      push_if_supported(instructions, i::bti, 
            "hw.optional.arm.FEAT_BTI");
      push_if_supported(instructions, i::fp_sync_exception, 
            "hw.optional.arm.FP_SyncExceptions");
      push_if_supported(instructions, i::neon, 
            "hw.optional.neon");
      push_if_supported(instructions, i::armv8_1_atomics, 
            "hw.optional.armv8_1_atomics");
      push_if_supported(instructions, i::armv8_2_fhm, 
            "hw.optional.armv8_2_fhm");
      push_if_supported(instructions, i::armv8_2_compnum, 
            "hw.optional.armv8_2_compnum");
      push_if_supported(instructions, i::watchpoint, 
            "hw.optional.watchpoint");
      push_if_supported(instructions, i::breakpoint, 
            "hw.optional.breakpoint");
      push_if_supported(instructions, i::armv8_crc32, 
            "hw.optional.armv8_crc32");
      push_if_supported(instructions, i::armv8_gpi, 
            "hw.optional.armv8_gpi");
      push_if_supported(instructions, i::adv_simd, 
            "hw.optional.AdvSIMD");
      push_if_supported(instructions, i::adv_simd_hpfp_cvt, 
            "hw.optional.AdvSIMD_HPFPCvt");
      push_if_supported(instructions, i::ucnormal_mem, 
            "hw.optional.ucnormal_mem");
      return instructions;
}
#endif /* __APPLE__ && INFOWARE_ARM */
