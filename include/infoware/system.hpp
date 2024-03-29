// SPDX-License-Identifier: CC0-1.0
// infoware - C++ System information Library


#pragma once


#include <infoware/linkage.hpp>

#include <cstdint>
#include <string>
#include <vector>

// Some garbage compilers define this, doesn't seem deterministic; see https://github.com/ThePhD/infoware/issues/42
#ifdef linux
#undef linux
#endif


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
			double refresh_rate;
		};

		struct display_config_t {
			std::uint32_t width;
			std::uint32_t height;
			std::vector<double> refresh_rates;
		};

		/// Get amount of connected mice.
		INFOWARE_API_LINKAGE std::size_t mouse_amount() noexcept;

		/// Get amount of connected keyboards.
		///
		/// Always returns 0 on Linuxish kernels, as it can not be detected there.
		INFOWARE_API_LINKAGE std::size_t keyboard_amount() noexcept;

		/// Get amount of other connected HIDs.
		///
		/// Always returns 0 on Linuxish kernels, as it can not be detected there.
		INFOWARE_API_LINKAGE std::size_t other_HID_amount() noexcept;

		/// Get RAM statistics.
		INFOWARE_API_LINKAGE memory_t memory() noexcept;

		/// Get kernel information.
		INFOWARE_API_LINKAGE kernel_info_t kernel_info();

		/// Get system information.
		INFOWARE_API_LINKAGE OS_info_t OS_info();

		/// Get information about displays.
		INFOWARE_API_LINKAGE std::vector<display_t> displays();

		/// Get information about available configurations for each display.
		INFOWARE_API_LINKAGE std::vector<std::vector<display_config_t>> available_display_configurations();
	}  // namespace system
}  // namespace iware
