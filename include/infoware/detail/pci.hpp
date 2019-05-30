// infoware - C++ System information Library
//
// Written in 2016-2019 by nabijaczleweli <nabijaczleweli@gmail.com> and ThePhD <phdofthehouse@gmail.com>
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
	namespace detail {
		struct pci_device_id {
			const char* vendor_name;
			const char* device_name;
		};

		pci_device_id identify_device(std::int64_t vendor_pci_id, std::int64_t device_pci_id) noexcept;
		const char* identify_vendor(std::int64_t vendor_pci_id) noexcept;
	}  // namespace detail
}  // namespace iware
