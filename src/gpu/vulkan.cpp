// SPDX-License-Identifier: CC0-1.0
// infoware - C++ System information Library


#ifdef INFOWARE_USE_VULKAN


#include "infoware/detail/scope.hpp"
#include "infoware/gpu.hpp"
#include <iostream>
#include <numeric>
#include <vulkan/vulkan.h>


std::vector<iware::gpu::device_properties_t> iware::gpu::device_properties() {
	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

	VkInstance instance;
	VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
	if(result != VK_SUCCESS)
		throw nullptr;
	iware::detail::quickscope_wrapper instance_deleter{[&]() { vkDestroyInstance(instance, nullptr); }};


	uint32_t devices_len = 2;
	std::vector<VkPhysicalDevice> devices(devices_len);
	while((result = vkEnumeratePhysicalDevices(instance, &devices_len, devices.data())) == VK_INCOMPLETE) {
		devices.emplace_back();
		devices_len = devices.size();
	}
	if(result != VK_SUCCESS)
		return {};
	devices.resize(devices_len);

	std::vector<iware::gpu::device_properties_t> ret;
	ret.reserve(devices.size());
	for(auto&& device : devices) {
		VkPhysicalDeviceProperties props{};
		vkGetPhysicalDeviceProperties(device, &props);

		// https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkPhysicalDeviceProperties.html
		// > If the vendor has a PCI vendor ID, the low 16 bits of vendorID must contain that PCI vendor ID, and the remaining bits must be set to zero.
		// > Otherwise, the value returned must be a valid Khronos vendor ID,
		// None of the <https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkVendorId.html>s match anything in our enum.
		auto vendor = vendor_t::unknown;
		switch(props.vendorID) {
			case 0x8086:  // Intel Corporation
				vendor = vendor_t::intel;
				break;
			case 0x1002:  // Advanced Micro Devices, Inc. [AMD/ATI]
			case 0x1022:  // Advanced Micro Devices, Inc. [AMD]
				vendor = vendor_t::amd;
				break;
			case 0x10de:  // NVIDIA Corporation
			case 0x12d2:  // NVidia / SGS Thomson (Joint Venture)
				vendor = vendor_t::nvidia;
				break;
			case 0x1414:  // Microsoft Corporation
				vendor = vendor_t::microsoft;
				break;
			case 0x168c:  // Qualcomm Atheros
			case 0x17cb:  // Qualcomm
			case 0x1969:  // Qualcomm Atheros
			case 0x5143:  // Qualcomm Inc
				vendor = vendor_t::qualcomm;
				break;
			case 0x106b:  // Apple Inc.
				vendor = vendor_t::apple;
				break;
		}

		VkPhysicalDeviceMemoryProperties mem{};
		vkGetPhysicalDeviceMemoryProperties(device, &mem);

		ret.emplace_back(device_properties_t{
		    vendor,
		    props.deviceName,
		    std::accumulate(mem.memoryHeaps, mem.memoryHeaps + mem.memoryHeapCount, static_cast<std::size_t>(0), [](auto i, auto&& heap) { return i + heap.size; }),
		    0,
		    0,
		});
	}
	return ret;
}


#endif
