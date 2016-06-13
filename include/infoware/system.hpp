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

#include <infoware/detail/api.hpp>
#include <cstdint>
#include <vector>


namespace iware {
	namespace system {
		struct memory_t {
			uint64_t physical_available;
			uint64_t physical_total;
			uint64_t virtual_available;
			uint64_t virtual_total;
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
		infoware_api unsigned int mouse_amount() noexcept;

		/// Get amount of connected keyboards.
		///
		/// Always returns 0 on Linuxish kernels, as it can not be detected there.
		infoware_api unsigned int keyboard_amount() noexcept;

		/// Get amount of other connected HIDs.
		///
		/// Always returns 0 on Linuxish kernels, as it can not be detected there.
		infoware_api unsigned int other_HID_amount() noexcept;

		/// Get RAM statistics.
		infoware_api memory_t memory() noexcept;

		/// Get kernel information.
		infoware_api kernel_info_t kernel_info() noexcept;

		/// Get information about displays.
		infoware_api std::vector<display_t> displays();
	}
}
