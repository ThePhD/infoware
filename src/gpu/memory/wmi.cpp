// infoware - C++ System information Library
//
// Written in 2018 by nabijaczleweli <nabijaczleweli@gmail.com> and ThePhD <phdofthehouse@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>


#ifndef INFOWARE_USE_D3D
#ifndef INFOWARE_USE_OPENCL
#ifdef INFOWARE_USE_WMI


#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include "infoware/detail/scope.hpp"
#include "infoware/gpu.hpp"
#include <iostream>
#include <string>
#include <wbemidl.h>
#include <windows.h>


/// Adapted from https://github.com/CherryPill/system_info/blob/aa85595b6cc2c63dedc98b6f983b94e8ec5c24e3/SystemInfo/sysinfo.cpp#L463
std::vector<iware::gpu::device_properties_t> iware::gpu::device_properties() {
	IWbemLocator* locator;

	HRESULT create_err =
	    CoCreateInstance(CLSID_WbemAdministrativeLocator, nullptr, CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER, IID_IUnknown, reinterpret_cast<void**>(&locator));

	if(SUCCEEDED(create_err)) {
		detail::quickscope_wrapper instance_deleter{[]() { CoUninitialize(); }};

		IWbemServices* services;
		locator->ConnectServer(SysAllocString(L"root\\CIMV2"), nullptr, nullptr, nullptr, 0, nullptr, nullptr, &services);
		locator->Release();
		detail::quickscope_wrapper services_deleter{[&]() { services->Release(); }};


		IEnumWbemClassObject* enumerator;
		const auto query_err = services->ExecQuery(SysAllocString(L"WQL"), SysAllocString(L"SELECT * FROM Win32_VideoController"),
		                                           WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, nullptr, &enumerator);
		if(FAILED(query_err))
			return {};
		detail::quickscope_wrapper enumerator_deleter{[&]() { enumerator->Release(); }};


		std::vector<iware::gpu::device_properties_t> ret;
		while(enumerator) {
			IWbemClassObject* object;
			ULONG next_err;
			enumerator->Next(WBEM_INFINITE, 1, &object, &next_err);
			if(next_err == 0)
				break;
			detail::quickscope_wrapper object_deleter{[&]() { object->Release(); }};

			VARIANT prop;
			BSTR name;
			while(SUCCEEDED(object->Next(0, &name, &prop, nullptr, nullptr))) {
				std::wcout << (name + 4) << L'\n';
				VariantClear(&prop);
			}
			std::wcout << L'\n';

			// VARIANT prop;
			// detail::quickscope_wrapper prop_deleter{[&]() { VariantClear(&prop); }};

			// object->Get(L"AdapterRAM", 0, &prop, 0, 0);
			// std::wstring vrammegabytesStr;

			// const std::size_t memory = prop.uintVal;

			// object->Get(L"Name", 0, &prop, 0, 0);
			// std::wstring name = prop.bstrVal;

			// ret.push_back(iware::gpu::device_properties_t{parse_vendor(vendorname), name, memory, cache});
		}

		return ret;
	} else
		return {};
}


#endif
#endif
#endif
