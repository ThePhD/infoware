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


#ifdef INFOWARE_USE_D3D


#include "infoware/detail/memory.hpp"
#include "infoware/detail/pci.hpp"
#include "infoware/gpu.hpp"
#include <codecvt>
#include <d3d11.h>
#include <locale>
#include <memory>
#include <tuple>


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

static std::string std_wide_to_string(const wchar_t* begin, const wchar_t* end) {
	return std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>{}.to_bytes(begin, end);
}


std::vector<iware::gpu::device_properties_t> iware::gpu::device_properties() {
	void* factory_raw;
	if(CreateDXGIFactory(__uuidof(IDXGIFactory), &factory_raw) != S_OK)
		return {};
	std::unique_ptr<IDXGIFactory, iware::detail::release_deleter> factory(static_cast<IDXGIFactory*>(factory_raw));

	std::vector<iware::gpu::device_properties_t> devices{};
	for(auto aid = 0u;; ++aid) {
		IDXGIAdapter* adapter_raw;
		const auto adapter_result = factory->EnumAdapters(aid, &adapter_raw);
		if(adapter_result != S_OK || adapter_result == DXGI_ERROR_NOT_FOUND)
			break;
		std::unique_ptr<IDXGIAdapter, iware::detail::release_deleter> adapter(static_cast<IDXGIAdapter*>(adapter_raw));

		DXGI_ADAPTER_DESC adapterdesc;
		adapter->GetDesc(&adapterdesc);

		auto device = iware::detail::identify_device(adapterdesc.VendorId, adapterdesc.DeviceId);
		if(device.device_name == "unknown")
			device.device_name = std_wide_to_string(adapterdesc.Description, adapterdesc.Description + std::char_traits<wchar_t>::length(adapterdesc.Description));
		devices.push_back({vendor_from_name(device.vendor_name), device.device_name, adapterdesc.DedicatedVideoMemory, adapterdesc.SharedSystemMemory});
	}
	return devices;
}


#endif
