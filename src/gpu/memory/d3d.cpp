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
#include <codecvt>

struct release_deleter {
	template <typename T>
	void operator()(T* p) const {
		p->Release();
	}
};

iware::gpu::vendor_t in_string(const std::string& v) {
	if (v.find("NVidia") != std::string::npos || v.find("NVIDIA") != std::string::npos) {
		return iware::gpu::vendor_t::NVidia;
	}
	else if (v.find("AMD") != std::string::npos || v.find("ATi") != std::string::npos || v.find("Advanced Micro Devices") != std::string::npos) {
		return iware::gpu::vendor_t::AMD;
	}
	else if (v.find("Intel") != std::string::npos) {
		return iware::gpu::vendor_t::intel;
	}
	else if (v.find("Microsoft") != std::string::npos) {
		return iware::gpu::vendor_t::microsoft;
	}
	else if (v.find("Qualcomm") != std::string::npos) {
		return iware::gpu::vendor_t::qualcomm;
	}
	return iware::gpu::vendor_t::unknown;
}

static std::string std_wide_to_string(const wchar_t* b, const wchar_t* e) {
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.to_bytes(b, e);
}

std::vector<iware::gpu::device_properties_t> iware::gpu::device_properties() {
	std::vector<iware::gpu::device_properties_t> devices{};

	std::unique_ptr<IDXGIFactory, release_deleter> factory;
	IDXGIFactory* pfactory = nullptr;
	void* vpfactory = static_cast<void*>(pfactory);
	HRESULT result = CreateDXGIFactory(__uuidof(IDXGIFactory), &vpfactory);
	if (result != S_OK) {
		// TODO: shit's lit, fam
		return devices;
	}
	pfactory = static_cast<IDXGIFactory*>(vpfactory);
	factory.reset(pfactory);

	for (std::size_t adp = 0;; ++adp) {
		std::unique_ptr<IDXGIAdapter, release_deleter> dxgiadapter = nullptr;
		IDXGIAdapter* pdxgiadapter = nullptr;
		result = factory->EnumAdapters(static_cast<UINT>(adp), &pdxgiadapter);
		if (result != S_OK || result == DXGI_ERROR_NOT_FOUND) {
			break;
		}
		dxgiadapter.reset(pdxgiadapter);
		DXGI_ADAPTER_DESC adapterdesc;
		dxgiadapter->GetDesc(&adapterdesc);
		std::string devicename, vendorname;
		std::tie( vendorname, devicename ) = detail::identify_device(adapterdesc.VendorId, adapterdesc.DeviceId);
		if (devicename == "unknown") {
			devicename = std_wide_to_string(adapterdesc.Description, adapterdesc.Description + std::char_traits<wchar_t>::length(adapterdesc.Description));
		}
		vendor_t vendortype = in_string(vendorname);
		devices.push_back({vendortype, devicename, adapterdesc.DedicatedVideoMemory, adapterdesc.SharedSystemMemory});
	}

	return devices;
}

#endif