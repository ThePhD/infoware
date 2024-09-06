// SPDX-License-Identifier: CC0-1.0
// infoware - C++ System information Library


#ifdef INFOWARE_USE_PCIIDS


#include "infoware/pci.hpp"
#include "infoware_generated/pci_data.hpp"
#include <algorithm>
#include <utility>


namespace {
	struct id_pair_t {
		std::uint64_t id;
		std::size_t index;
	};

	struct pci_vendor_info_t {
		std::uint64_t pci_id;
		const char* name;
		std::initializer_list<const std::uint64_t> device_indices;
	};

	struct pci_device_info_t {
		std::uint64_t pci_id;
		const char* name;
	};
}  // anonymous namespace


static const id_pair_t indices[]{INFOWARE_GENERATED_PCI_INDICES};
static const pci_vendor_info_t vendors[]{INFOWARE_GENERATED_PCI_VENDORS};
static const pci_device_info_t devices[]{INFOWARE_GENERATED_PCI_DEVICES};


static const pci_vendor_info_t* find_vendor(std::uint64_t vendor_id) noexcept {
	const auto idx_itr = std::lower_bound(std::begin(indices), std::end(indices), vendor_id, [](auto&& left, auto right) { return left.id < right; });
	if(idx_itr != std::end(indices) && idx_itr->id == vendor_id)
		return &vendors[idx_itr->index];
	else
		return nullptr;
}


iware::pci::device iware::pci::identify_device(std::uint64_t vendor_id, std::uint64_t device_id) noexcept {
	const auto vendor = find_vendor(vendor_id);
	if(!vendor)
		return {nullptr, nullptr};

	const auto device_itr = std::lower_bound(std::begin(vendor->device_indices), std::end(vendor->device_indices), device_id,
	                                         [](auto left, auto right) { return devices[left].pci_id < right; });
	if(device_itr == std::end(vendor->device_indices) || devices[*device_itr].pci_id != device_id)
		return {vendor->name, nullptr};

	const auto& device = devices[*device_itr];
	return {vendor->name, device.name};
}


const char* iware::pci::identify_vendor(std::uint64_t pci_id) noexcept {
	if(const auto vendor = find_vendor(pci_id))
		return vendor->name;
	else
		return nullptr;
}


#endif
