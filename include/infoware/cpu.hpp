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


		/// Returns the amount of processors available.
		///
		/// Each hypercore is considered a processor.
		unsigned int cores() noexcept;

		/// Returns the architecture of the current CPU.
		architecture_t architecture() noexcept;

		/// Returns the current frequency of the current CPU in Hz.
		unsigned int frequency() noexcept;

		/// Returns the current endianness of the current CPU.
		endianness_t endianness() noexcept;

		/// Returns the CPU's vendor.
		std::string vendor();
	}
}
