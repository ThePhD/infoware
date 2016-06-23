

#include <infoware/detail/cpuid.hpp>
#include <cstdint>

#if defined _WIN32

#include <intrin.h>

void iware::detail::cpuid(int32_t (&out)[4], int32_t x) {
	__cpuidex(out, x, 0);
}

std::uint64_t iware::detail::xgetbv(std::uint32_t x) {
	return _xgetbv(x);
}

#else

#include <cpuid.h>

void iware::detail::cpuid(std::int32_t (&out)[4], std::int32_t x) {
	__cpuid_count(x, 0, out[0], out[1], out[2], out[3]);
}

std::uint64_t iware::detail::xgetbv(std::uint32_t index) {
	std::uint32_t eax, edx;
	__asm__ __volatile__("xgetbv" : "=a"(eax), "=d"(edx) : "c"(index));
	return ((uint64_t)edx << 32) | eax;
}

#endif


