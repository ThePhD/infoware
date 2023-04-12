// SPDX-License-Identifier: CC0-1.0
// infoware - C++ System information Library


#pragma once


#include <infoware/linkage.hpp>

#include <cstdint>
#include <string>


namespace iware {
	namespace pci {
		struct device {
			const char* vendor_name;
			const char* device_name;
		};


		/// Get the names for the device with the specified PCI ID from the vendor with the specified PCI ID.
		///
		/// If the vendor was not found, both names are nullptr.
		/// If the vendor was found, but the device wasn't, the device name is nullptr;
		INFOWARE_API_LINKAGE device identify_device(std::uint64_t vendor_id, std::uint64_t device_id) noexcept;

		/// Get the name the vendor with the specified PCI ID, or nullptr if not found.
		INFOWARE_API_LINKAGE const char* identify_vendor(std::uint64_t vendor_id) noexcept;
	}  // namespace pci
}  // namespace iware
