// SPDX-License-Identifier: CC0-1.0
// infoware - C++ System information Library


#ifdef _WIN32


#include "infoware/detail/memory.hpp"
#include "infoware/detail/scope.hpp"
#include "infoware/detail/winstring.hpp"
#include "infoware/system.hpp"
#include <memory>
#include <string>
#define WIN32_LEAN_AND_MEAN
#include <wbemidl.h>
#include <windows.h>
#include <winnt.h>
#include <winternl.h>

#ifdef _MSC_VER
#define strtok_r(...) strtok_s(__VA_ARGS__)
#endif


// Use WIM to acquire Win32_OperatingSystem.Caption (same as .Name, but ends before the '|')
// https://msdn.microsoft.com/en-us/library/aa390423(v=vs.85).aspx
static std::string version_name() {
	auto err = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	if(err == RPC_E_CHANGED_MODE)  // COM already initialised as COINIT_APARTMENTTHREADED: must pass that to bump the reference count
		err = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
	if(FAILED(err))
		return {};
	iware::detail::quickscope_wrapper com_uninitialiser{CoUninitialize};

	const auto init_result =
	    CoInitializeSecurity(nullptr, -1, nullptr, nullptr, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, nullptr, EOAC_NONE, nullptr);
	if(FAILED(init_result) && init_result != RPC_E_TOO_LATE)
		return {};

	IWbemLocator* wbem_loc_raw;
	if(FAILED(CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, reinterpret_cast<void**>(&wbem_loc_raw))))
		return {};
	std::unique_ptr<IWbemLocator, iware::detail::release_deleter> wbem_loc(wbem_loc_raw);

	IWbemServices* wbem_services_raw;
	wchar_t network_resource[] = LR"(ROOT\CIMV2)";
	if(FAILED(wbem_loc->ConnectServer(network_resource, nullptr, nullptr, 0, 0, 0, 0, &wbem_services_raw)))
		return {};
	std::unique_ptr<IWbemServices, iware::detail::release_deleter> wbem_services(wbem_services_raw);

	if(FAILED(CoSetProxyBlanket(wbem_services.get(), RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, nullptr, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, nullptr,
	                            EOAC_NONE)))
		return {};

	IEnumWbemClassObject* query_iterator_raw;
	wchar_t query_lang[] = L"WQL";
	wchar_t query[]      = L"SELECT Caption FROM Win32_OperatingSystem";
	if(FAILED(wbem_services->ExecQuery(query_lang, query, WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, nullptr, &query_iterator_raw)))
		return {};
	std::unique_ptr<IEnumWbemClassObject, iware::detail::release_deleter> query_iterator(query_iterator_raw);

	std::string ret;
	while(query_iterator) {
		IWbemClassObject* value_raw;
		unsigned long iter_result;

		query_iterator->Next(static_cast<LONG>(WBEM_INFINITE), 1, &value_raw, &iter_result);
		if(!iter_result)
			break;
		std::unique_ptr<IWbemClassObject, iware::detail::release_deleter> value(value_raw);

		VARIANT val;
		VariantInit(&val);
		value->Get(L"Caption", 0, &val, 0, 0);
		iware::detail::quickscope_wrapper val_destructor{[&] { VariantClear(&val); }};

		ret = iware::detail::narrowen_bstring(val.bstrVal);
	}

	return ret;
}

static unsigned int build_number() {
	HKEY hkey{};
	if(RegOpenKeyExA(HKEY_LOCAL_MACHINE, R"(Software\Microsoft\Windows NT\CurrentVersion)", 0, KEY_READ, &hkey))
		return {};
	iware::detail::quickscope_wrapper hkey_closer{[&] { RegCloseKey(hkey); }};

	DWORD ubr{};
	DWORD ubr_size = sizeof(ubr);
	if(!RegQueryValueExA(hkey, "UBR", nullptr, nullptr, reinterpret_cast<LPBYTE>(&ubr), &ubr_size))
		return ubr;

	// Fall back to BuildLabEx in the early version of Windows 8.1 and less.
	DWORD buildlabex_size{};
	if(RegQueryValueExA(hkey, "BuildLabEx", nullptr, nullptr, nullptr, &buildlabex_size))
		return {};

	std::string buildlabex(buildlabex_size, {});  // REG_SZ may not be NUL-terminated
	if(RegQueryValueExA(hkey, "BuildLabEx", nullptr, nullptr, reinterpret_cast<LPBYTE>(&buildlabex[0]), &buildlabex_size))
		return {};

	char* ctx{};
	auto token = strtok_r(&buildlabex[0], ".", &ctx);
	token      = strtok_r(nullptr, ".", &ctx);
	return token ? std::strtoul(token, nullptr, 10) : 0;
}

// RtlGetVersion() started being phased out in Windows 8; need to load it dynamically for Windows 11(?)
// https://docs.microsoft.com/en-us/windows/win32/devnotes/rtlgetversion
iware::system::OS_info_t iware::system::OS_info() {
	static NTSTATUS(NTAPI * pRtlGetVersion)(_Out_ PRTL_OSVERSIONINFOW lpVersionInformation) = [] {
		decltype(pRtlGetVersion) ret{};
		if(HMODULE ntdllh = GetModuleHandleA("ntdll.dll"))
			ret = reinterpret_cast<decltype(pRtlGetVersion)>(GetProcAddress(ntdllh, "RtlGetVersion"));
		return ret;
	}();
	RTL_OSVERSIONINFOW os_version_info{};
	os_version_info.dwOSVersionInfoSize = sizeof(os_version_info);
	if(pRtlGetVersion)
		pRtlGetVersion(&os_version_info);

	return {"Windows NT", version_name(), os_version_info.dwMajorVersion, os_version_info.dwMinorVersion, os_version_info.dwBuildNumber, build_number()};
}


#endif
