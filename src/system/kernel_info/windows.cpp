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


#ifdef _WIN32


#include "infoware/system.hpp"
#include <memory>
#include <string>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>


iware::system::kernel_info_t iware::system::kernel_info() {
	std::string path;
	path.resize(GetSystemDirectory(nullptr, 0) - 1);
	GetSystemDirectory(&path[0], path.size() + 1);
	path += "\\kernel32.dll";


	const auto ver_info_len = GetFileVersionInfoSize(path.c_str(), nullptr);
	auto ver_info           = std::make_unique<std::uint8_t[]>(ver_info_len);
	GetFileVersionInfo(path.c_str(), 0, ver_info_len, ver_info.get());

	VS_FIXEDFILEINFO* file_version;
	unsigned int file_version_len;
	VerQueryValue(ver_info.get(), "", reinterpret_cast<void**>(&file_version), &file_version_len);

	return {iware::system::kernel_t::windows_nt, HIWORD(file_version->dwProductVersionMS), LOWORD(file_version->dwProductVersionMS),
	        HIWORD(file_version->dwProductVersionLS), LOWORD(file_version->dwProductVersionLS)};
}


#endif
