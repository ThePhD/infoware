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


#pragma once


#include <cstdint>
#include <string>


namespace iware {
	namespace cpu {
		enum class architecture_t {
			x64,
			ARM,
			itanium,
			x86,
			unknown,
		};

		enum class endianness_t {
			little,
			big,
			middle,
		};

		enum class instruction_set_t {
			ThreeD_now,
			MMX,
			SSE,
			SSE2,
			SSE3,
			AVX,
		};

		enum class cache_type_t {
			unified,
			instruction,
			data,
			trace,
		};

		struct quantities_t {
			/// Hyperthreads.
			std::uint32_t logical;
			/// Physical "cores".
			std::uint32_t physical;
			/// Physical CPU units/packages/sockets.
			std::uint32_t packages;
		};

		struct cache_t {
			std::size_t size;
			std::size_t line_size;
			std::uint8_t associativity;
			cache_type_t type;
		};


		/// Returns the quantity of CPU at various gradation.
		quantities_t quantities();

		/// Get CPU's cache properties.
		///
		/// `level` is the cache level (3 -> L3 cache).
		cache_t cache(unsigned int level);

		/// Returns the architecture of the current CPU.
		architecture_t architecture() noexcept;

		/// Returns the current frequency of the current CPU in Hz.
		std::int64_t frequency() noexcept;

		/// Returns the current endianness of the current CPU.
		endianness_t endianness() noexcept;

		/// Returns the CPU's vendor.
		std::string vendor();

		/// Returns the CPU's model name.
		std::string model_name();

		/// Returns whether an instruction set is supported by the current CPU.
		///
		/// `noexcept` on Windows
		bool instruction_set_supported(instruction_set_t set);
	}
}
