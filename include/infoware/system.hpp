// infoware - C++ System information Library
//
// Written in 2017 by nabijaczleweli <nabijaczleweli@gmail.com> and ThePhD <phdofthehouse@gmail.com>
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
#include <vector>


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
			std::uint32_t major;
			std::uint32_t minor;
			std::uint32_t patch;
			std::uint32_t build_number;
		};

		struct OS_info_t {
			std::string name;
			std::string full_name;
			unsigned int major;
			unsigned int minor;
			unsigned int patch;
			unsigned int build_number;
		};

		struct display_t {
			std::uint32_t width;
			std::uint32_t height;
			std::uint32_t dpi;
			/// Bits Per Pixel a.k.a. depth
			std::uint32_t bpp;
		};


		/// Get amount of connected mice.
		std::size_t mouse_amount() noexcept;

		/// Get amount of connected keyboards.
		///
		/// Always returns 0 on Linuxish kernels, as it can not be detected there.
		std::size_t keyboard_amount() noexcept;

		/// Get amount of other connected HIDs.
		///
		/// Always returns 0 on Linuxish kernels, as it can not be detected there.
		std::size_t other_HID_amount() noexcept;

		/// Get RAM statistics.
		memory_t memory() noexcept;

		/// Get kernel information.
		kernel_info_t kernel_info();

		/// Get kernel information.
		OS_info_t OS_info();

		/// Get information about displays.
		std::vector<display_t> displays();
	}
}
