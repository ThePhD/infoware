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


#include "infoware/system.hpp"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>


// https://msdn.microsoft.com/en-us/library/windows/desktop/ms645598(v=vs.85).aspx
// https://msdn.microsoft.com/en-us/library/windows/desktop/ms645568(v=vs.85).aspx
static std::size_t device_amount(unsigned int device_type) noexcept {
	// Assuming 64 is enough for anyone
	RAWINPUTDEVICELIST input_devices[64];
	unsigned int input_devices_n = sizeof(input_devices) / sizeof(RAWINPUTDEVICELIST);
	const auto amt               = GetRawInputDeviceList(input_devices, &input_devices_n, sizeof(RAWINPUTDEVICELIST));

	auto result = 0u;
	for(auto i = 0u; i < amt; ++i)
		result += input_devices[i].dwType == device_type;
	return result;
}


std::size_t iware::system::mouse_amount() noexcept {
	return device_amount(RIM_TYPEMOUSE);
}

std::size_t iware::system::keyboard_amount() noexcept {
	return device_amount(RIM_TYPEKEYBOARD);
}

std::size_t iware::system::other_HID_amount() noexcept {
	return device_amount(RIM_TYPEHID);
}


#endif
