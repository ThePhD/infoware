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

#ifndef _WIN32

std::vector<iware::gpu::device_properties_t> iware::gpu::device_properties() {
	// TODO: the implementation details for hitting the hardware directly through the OS and somesuch
	// are going to go here
	return{};
}

#endif // Not-Windows
