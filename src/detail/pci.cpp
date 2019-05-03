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


#include "infoware/detail/pci.hpp"
#include "infoware_generated/pci_data.hpp"
#include <algorithm>
#include <utility>


namespace {
	struct id_pair_t {
		int64_t id;
		std::size_t index;
	};

	template <typename T>
	struct cheap_view {
		T* arr;
		std::size_t arr_size;
		T* begin() const { return arr; }
		T* end() const { return arr + arr_size; }
	};

	struct pci_vendor_info_t {
		int64_t pci_id;
		cheap_view<const char> name;
		std::initializer_list<const int64_t> device_indices;
	};

	struct pci_device_info_t {
		int64_t pci_id;
		cheap_view<const char> name;
	};
}  // namespace


iware::detail::pci_device_id iware::detail::identify_device(int64_t vendor_pci_id, int64_t device_pci_id) noexcept {
	static const id_pair_t indices[]{INFOWARE_GENERATED_PCI_INDICES};
	static const pci_vendor_info_t vendors[]{INFOWARE_GENERATED_PCI_VENDORS};
	static const pci_device_info_t devices[]{INFOWARE_GENERATED_PCI_DEVICES};


	const auto idx_itr = std::lower_bound(std::begin(indices), std::end(indices), vendor_pci_id, [](auto&& left, auto right) { return left.id < right; });
	if(idx_itr == std::end(indices) || idx_itr->id != vendor_pci_id)
		return {"unknown", "unknown"};

	const auto& vendor    = vendors[idx_itr->index];
	const auto device_itr = std::lower_bound(std::begin(vendor.device_indices), std::end(vendor.device_indices), device_pci_id,
	                                         [](auto left, auto right) { return devices[left].pci_id < right; });
	if(device_itr == std::end(vendor.device_indices) || devices[*device_itr].pci_id != device_pci_id)
		return {vendor.name.arr, "unknown"};

	const auto& device = devices[*device_itr];
	return {vendor.name.arr, device.name.arr};
}


std::string iware::detail::identify_vendor(int64_t pci_id) noexcept {
	return identify_device(pci_id, 0).vendor_name;
}
