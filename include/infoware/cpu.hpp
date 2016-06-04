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
		};

		enum class instruction_set_t {
			/// 3D-now
			ThreeD_now,
			MMX,
			SSE,
			SSE2,
			SSE3,
		};

		struct quantities_t {
			/// Hyperthreads.
			unsigned int logical;
			/// Physical "cores".
			unsigned int physical;
			/// Physical CPU units/packages/sockets.
			unsigned int packages;
		};


		/// Returns the quantity of CPU at various gradation.
		quantities_t quantities();

		/// Returns the architecture of the current CPU.
		architecture_t architecture() noexcept;

		/// Returns the current frequency of the current CPU in Hz.
		unsigned int frequency() noexcept;

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
