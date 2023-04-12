// SPDX-License-Identifier: CC0-1.0
// infoware - C++ System information Library


#ifdef INFOWARE_USE_D3D


#include "infoware/detail/memory.hpp"
#include "infoware/detail/winstring.hpp"
#include "infoware/gpu.hpp"
#include "infoware/pci.hpp"
#include <d3d11.h>
#include <memory>


static iware::gpu::vendor_t vendor_from_name(const std::string& v) {
	if(v.find("NVidia") != std::string::npos || v.find("NVIDIA") != std::string::npos)
		return iware::gpu::vendor_t::nvidia;
	else if(v.find("AMD") != std::string::npos || v.find("ATi") != std::string::npos || v.find("Advanced Micro Devices") != std::string::npos)
		return iware::gpu::vendor_t::amd;
	else if(v.find("Intel") != std::string::npos)
		return iware::gpu::vendor_t::intel;
	else if(v.find("Microsoft") != std::string::npos)
		return iware::gpu::vendor_t::microsoft;
	else if(v.find("Qualcomm") != std::string::npos)
		return iware::gpu::vendor_t::qualcomm;
	else
		return iware::gpu::vendor_t::unknown;
}


std::vector<iware::gpu::device_properties_t> iware::gpu::device_properties() {
	IDXGIFactory* factory_raw;
	if(CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&factory_raw)) != S_OK)
		return {};
	std::unique_ptr<IDXGIFactory, iware::detail::release_deleter> factory(factory_raw);

	std::vector<iware::gpu::device_properties_t> devices{};
	for(auto aid = 0u;; ++aid) {
		IDXGIAdapter* adapter_raw;
		const auto adapter_result = factory->EnumAdapters(aid, &adapter_raw);
		if(adapter_result != S_OK || adapter_result == DXGI_ERROR_NOT_FOUND)
			break;
		std::unique_ptr<IDXGIAdapter, iware::detail::release_deleter> adapter(adapter_raw);

		DXGI_ADAPTER_DESC adapterdesc;
		adapter->GetDesc(&adapterdesc);

		const auto device       = iware::pci::identify_device(adapterdesc.VendorId, adapterdesc.DeviceId);
		std::string device_name = device.device_name ? device.device_name : iware::detail::narrowen_winstring(adapterdesc.Description);

		devices.push_back({vendor_from_name(device.vendor_name), device_name, adapterdesc.DedicatedVideoMemory, adapterdesc.SharedSystemMemory,
		                   // TODO: there's purportedly (https://en.wikipedia.org/wiki/Windows_Display_Driver_Model#WDDM_2.3)
		                   //       a Windows API for getting the max clock, but I haven't been able to find it or use it
		                   0});
	}
	return devices;
}


#endif
