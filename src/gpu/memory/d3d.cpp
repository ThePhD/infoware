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


#include <infoware/gpu.hpp>

#if defined(INFOWARE_USE_D3D)

#include <infoware/detail/pci.hpp>
#include <d3d11.h>
#include <memory>
#include <locale>
#include <tuple>

struct release_deleter {
	template <typename T>
	void operator()(T* p) const {
		p->Release();
	}
};

iware::gpu::vendor_t in_string(const std::string& v) {
	if (v.find("NVidia") != std::string::npos) {
		return iware::gpu::vendor_t::nvidia;
	}
	else if (v.find("AMD") != std::string::npos || v.find("ATi") != std::string::npos || v.find("Advanced Micro Devices") != std::string::npos) {
		return iware::gpu::vendor_t::amd;
	}
	else if (v.find("Intel") != std::string::npos) {
		return iware::gpu::vendor_t::intel;
	}
	return iware::gpu::vendor_t::unknown;
}

std::vector<iware::gpu::device_properties_t> iware::gpu::device_properties() {
	std::vector<iware::gpu::device_properties_t> devices{};

	std::unique_ptr<IDXGIFactory, release_deleter> factory;
	IDXGIFactory* pfactory = nullptr;
	void** vppfactory = reinterpret_cast<void**>(pfactory);
	HRESULT result = CreateDXGIFactory(__uuidof(IDXGIFactory), vppfactory);
	if (result != S_OK) {
		// TODO: shit's lit, fam
		return devices;
	}
	factory.reset(pfactory);

	for (std::size_t adp = 0;; ++adp) {
		std::unique_ptr<IDXGIAdapter, release_deleter> dxgiadapter = nullptr;
		IDXGIAdapter* pdxgiadapter = nullptr;
		result = factory->EnumAdapters(static_cast<UINT>(adp), &pdxgiadapter);
		if (result != S_OK) {
			break;
		}
		DXGI_ADAPTER_DESC adapterdesc;
		dxgiadapter->GetDesc(&adapterdesc);
		std::string devicename, vendorname;
		std::tie( vendorname, devicename ) = detail::identify_device(adapterdesc.VendorId, adapterdesc.DeviceId);
		devices.push_back({vendor_t::unknown, std::move(vendorname), std::move(devicename), adapterdesc.DedicatedVideoMemory, 0, adapterdesc.DedicatedSystemMemory, adapterdesc.SharedSystemMemory});
	}

	return devices;
}

#endif