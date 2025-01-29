// SPDX-License-Identifier: CC0-1.0
// infoware - C++ System information Library


#ifdef _WIN32


#include "infoware/cpu.hpp"
#include "infoware/detail/scope.hpp"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>


template <unsigned int IdentLen>
static std::string central_processor_subkey(const char* key) {
	HKEY hkey;
	if(RegOpenKeyExA(HKEY_LOCAL_MACHINE, R"(HARDWARE\DESCRIPTION\System\CentralProcessor\0)", 0, KEY_READ, &hkey))
		return {};

	iware::detail::quickscope_wrapper hkey_closer{[&] { RegCloseKey(hkey); }};

	char identifier[IdentLen];
	DWORD identifier_len = sizeof(identifier);
	LPBYTE lpdata        = static_cast<LPBYTE>(static_cast<void*>(&identifier[0]));
	if(RegQueryValueExA(hkey, key, nullptr, nullptr, lpdata, &identifier_len))
		return {};

	return identifier;
}

std::string iware::cpu::vendor() {
	return central_processor_subkey<12 + 1>("VendorIdentifier");
}

std::string iware::cpu::model_name() {
	return central_processor_subkey<64 + 1>("ProcessorNameString");
}


#endif
