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
#include <vector>
#include <string>

namespace iware {
	namespace system {
		struct memory_t {
			std::uint64_t physical_available;
			std::uint64_t physical_total;
			std::uint64_t virtual_available;
			std::uint64_t virtual_total;
		};

		enum class kernel_t {
			windows_nt,
			linux,
			darwin,
			unknown,
		};

		struct kernel_info_t {
			kernel_t variant;
			unsigned int major;
			unsigned int minor;
			unsigned int patch;
			unsigned int build_number;
		};

		struct display_t {
			unsigned int width;
			unsigned int height;
			unsigned int dpi;
			/// Bits Per Pixel a.k.a. depth
			unsigned int bpp;
		};


		/// Get amount of connected mice.
		unsigned int mouse_amount() noexcept;

		/// Get amount of connected keyboards.
		///
		/// Always returns 0 on Linuxish kernels, as it can not be detected there.
		unsigned int keyboard_amount() noexcept;

		/// Get amount of other connected HIDs.
		///
		/// Always returns 0 on Linuxish kernels, as it can not be detected there.
		unsigned int other_HID_amount() noexcept;

		/// Get RAM statistics.
		memory_t memory() noexcept;

		/// Get kernel information.
		kernel_info_t kernel_info();

		/// Get information about displays.
		std::vector<display_t> displays();
	}
}
