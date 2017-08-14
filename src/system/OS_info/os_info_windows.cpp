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


#ifdef _WIN32


#include "infoware/detail/memory.hpp"
#include "infoware/detail/scope.hpp"
#include "infoware/system.hpp"
#include <memory>
#include <string>
#define WIN32_LEAN_AND_MEAN
#include <wbemidl.h>
#include <windows.h>


// Adapted from
// https://gitlab.isb-sib.ch/itopolsk/captain-bol/blob/master/xenobol/src/comutil.h
static std::wstring ConvertStringToBSTR(const char* pSrc) {
	if(!pSrc)
		return {};

	std::wstring ret;
	if(const auto cwch = MultiByteToWideChar(CP_ACP, 0, pSrc, -1, nullptr, 0)) {
		ret.resize(cwch - 1);
		if(!MultiByteToWideChar(CP_ACP, 0, pSrc, -1, &ret[0], static_cast<int>(ret.size())))
			if(ERROR_INSUFFICIENT_BUFFER == GetLastError())
				return ret;
	}
	return ret;
}

static std::string ConvertBSTRToString(BSTR pSrc) {
	if(!pSrc)
		return {};

	// convert even embeded NUL
	const auto src_len = SysStringLen(pSrc);
	std::string ret;
	if(const auto len = WideCharToMultiByte(CP_ACP, 0, pSrc, src_len + 1, NULL, 0, 0, 0)) {
		ret.resize(len + 1, '\0');
		if(!WideCharToMultiByte(CP_ACP, 0, pSrc, src_len + 1, &ret[0], len, 0, 0))
			ret.clear();
	}
	return ret;
}


// Use WIM to acquire Win32_OperatingSystem.Name
// https://msdn.microsoft.com/en-us/library/aa390423(v=vs.85).aspx
static std::string version_name() {
	if(FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED)))
		return {};
	iware::detail::quickscope_wrapper com_uninitialiser{CoUninitialize};

	if(FAILED(CoInitializeSecurity(nullptr, -1, nullptr, nullptr, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, nullptr, EOAC_NONE, nullptr)))
		return {};

	IWbemLocator* webm_loc_raw;
	if(FAILED(CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, reinterpret_cast<void**>(&webm_loc_raw))))
		return {};
	std::unique_ptr<IWbemLocator, iware::detail::release_deleter> webm_loc(webm_loc_raw);

	IWbemServices* webm_services_raw;
	if(FAILED(webm_loc->ConnectServer(&ConvertStringToBSTR("ROOT\\CIMV2")[0], nullptr, nullptr, 0, 0, 0, 0, &webm_services_raw)))
		return {};
	std::unique_ptr<IWbemServices, iware::detail::release_deleter> webm_services(webm_services_raw);

	if(FAILED(CoSetProxyBlanket(webm_services.get(), RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, nullptr, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, nullptr,
	                            EOAC_NONE)))
		return {};

	IEnumWbemClassObject* query_iterator_raw;
	if(FAILED(webm_services->ExecQuery(&ConvertStringToBSTR("WQL")[0], &ConvertStringToBSTR("SELECT Name FROM Win32_OperatingSystem")[0],
	                                   WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, nullptr, &query_iterator_raw)))
		return {};
	std::unique_ptr<IEnumWbemClassObject, iware::detail::release_deleter> query_iterator(query_iterator_raw);

	std::string ret;
	while(query_iterator) {
		IWbemClassObject* value_raw;
		unsigned long iter_result;

		query_iterator->Next(WBEM_INFINITE, 1, &value_raw, &iter_result);
		if(!iter_result)
			break;
		std::unique_ptr<IWbemClassObject, iware::detail::release_deleter> value(value_raw);

		VARIANT val;
		value->Get(L"Name", 0, &val, 0, 0);
		iware::detail::quickscope_wrapper val_destructor{[&] { VariantClear(&val); }};
		ret = ConvertBSTRToString(val.bstrVal);
	}
	return ret.substr(0, ret.find('|'));
}


iware::system::OS_info_t iware::system::OS_info() {
	const auto kernel_version = iware::system::kernel_info();
	return {"Windows NT", version_name(), kernel_version.major, kernel_version.minor, kernel_version.patch, kernel_version.build_number};
}


#endif
