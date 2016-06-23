#pragma once


#include <cstdint>

namespace iware {
	namespace detail {

		void cpuid(int32_t(&out)[4], int32_t x);

		std::uint64_t xgetbv(std::uint32_t x);

	}
}

