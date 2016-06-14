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

#include <infoware/detail/os.hpp>


#ifdef INFOWARE_WIN


#include "infoware/system.hpp"
#include <memory>
#include <cstddef>
#include <sstream>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <VersionHelpers.h>


iware::system::kernel_info_t iware::system::kernel_info() noexcept {
#if 0 // Last resort if VC++ continues to be an ASS with its sdl checks
	OSVERSIONINFOEXW osinfoexw{ sizeof(OSVERSIONINFOEXW) };
	if (GetVersionExW(reinterpret_cast<OSVERSIONINFOW*>(&osinfoexw)) == 0) {
		// Shit hit the fan, fam...
	}

	unsigned int major_version = osinfoexw.dwMajorVersion;
	unsigned int minor_version = osinfoexw.dwMinorVersion;
	unsigned int build = osinfoexw.dwBuildNumber;

	return{ iware::system::kernel_t::windows_nt, major_version, minor_version, 0, build };
#else
	static const auto& osfiletarget = L"\\kernel32.dll";
	static const std::size_t osfiletargetsize = sizeof(osfiletargetsize) / sizeof(L"");
	static const std::size_t pathsize = _MAX_PATH + 14;

	// zero-init all entries with {}
	WCHAR path[pathsize]{};

	UINT getsysdirresult = GetSystemDirectoryW(path, pathsize);
	if (getsysdirresult == 0 || getsysdirresult >= pathsize) {
		// TODO: Shit hit the fan, fam
	}
	std::copy(osfiletarget, osfiletarget + osfiletargetsize, path);

	//
	// Based on example code from this article
	// http://support.microsoft.com/kb/167597
	//

	DWORD handle{};
#if (_WIN32_WINNT >= _WIN32_WINNT_VISTA)
	DWORD len = GetFileVersionInfoSizeExW(FILE_VER_GET_NEUTRAL, path, &handle);
#else
	DWORD len = GetFileVersionInfoSizeW(path, &handle);
#endif
	if (!len) {
		// TODO: shit's lit, fam
	}

	std::unique_ptr<uint8_t[]> buff(new (std::nothrow) uint8_t[len]);
	if (!buff) {
		// TODO: shit's lit, fam
	}

#if (_WIN32_WINNT >= _WIN32_WINNT_VISTA)
	if (!GetFileVersionInfoExW(FILE_VER_GET_NEUTRAL, path, 0, len, buff.get()) == 0) {
#else
	if (!GetFileVersionInfoW(path, 0, len, buff.get()) == 0) {
#endif
		// TODO: shit's lit, fam
	}

	VS_FIXEDFILEINFO *vInfo = nullptr;
	UINT infoSize;

	if (!VerQueryValueW(buff.get(), L"\\", reinterpret_cast<LPVOID*>(&vInfo), &infoSize)) {
		// TODO: shit's lit, fam
	}

	if (!infoSize) {
		// TODO: shit's lit, fam
	}
	uint32_t major = HIWORD(vInfo->dwFileVersionMS);
	uint32_t minor = LOWORD(vInfo->dwFileVersionMS);
	uint32_t patch = HIWORD(vInfo->dwFileVersionLS);
	uint32_t build = LOWORD(vInfo->dwFileVersionLS);
	std::stringstream name;
	name << "Windows NT " << major << "." << minor << "." << patch << "." << build;

	iware::system::kernel_info_t ki{ kernel_t::windows_nt, major, minor, patch, build, name.str(), {} };
	
	bool is_server = IsWindowsServer();
#ifdef _WIN32_WINNT_WIN10 
	// Control depending in Windows 10 SDK use
	if (IsWindows10OrGreater()) {
		ki.friendly_name = "Windows 10";
	}
#endif
#ifdef _WIN32_WINNT_WIN8
	// Control depending in Windows 8 SDK Use
	else if (IsWindows8Point1OrGreater()) {
		ki.friendly_name = "Windows 8.1";
	}
	else if (IsWindows8OrGreater()) {
		ki.friendly_name = "Windows 8";
	}
#endif
	else if (IsWindows7SP1OrGreater()) {
		ki.friendly_name = "Windows 7 Service Pack 1";
	}
	else if (IsWindows7OrGreater()) {
		ki.friendly_name = "Windows 7";
	}
	else if (IsWindowsXPSP3OrGreater()) {
		ki.friendly_name = "Windows XP Service Pack 3";
	}
	else if (IsWindowsXPSP2OrGreater()) {
		ki.friendly_name = "Windows XP Service Pack 2";
	}
	else if (IsWindowsXPSP1OrGreater()) {
		ki.friendly_name = "Windows XP Service Pack 1";
	}
	else if (IsWindowsXPOrGreater()) {
		ki.friendly_name = "Windows XP";
	}
	return ki;
#endif
}


#endif
