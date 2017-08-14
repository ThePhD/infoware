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


#include "infoware/gpu.hpp"
#include <iostream>


static const char* vendor_name(iware::gpu::vendor_t vendor) noexcept;


int main(int, char*[]) {
	{
		const auto device_properties = iware::gpu::device_properties();
		std::cout << "\n"
		             "  Properties:\n";
		if(device_properties.empty())
			std::cout << "    No detection methods enabled\n";
		else
			for(auto i = 0u; i < device_properties.size(); ++i) {
				const auto& properties_of_device = device_properties[i];
				std::cout << "    Device #" << (i + 1) << ":\n"
				          << "      Vendor    : " << vendor_name(properties_of_device.vendor) << '\n'
				          << "      Name      : " << properties_of_device.name << '\n'
				          << "      RAM size  : " << properties_of_device.memory_size << "B\n"
				          << "      Cache size: " << properties_of_device.cache_size << "B\n";
			}
	}

	std::cout << '\n';

	return 0;
}


static const char* vendor_name(iware::gpu::vendor_t vendor) noexcept {
	switch(vendor) {
		case iware::gpu::vendor_t::intel:
			return "Intel";
		case iware::gpu::vendor_t::amd:
			return "AMD";
		case iware::gpu::vendor_t::nvidia:
			return "NVidia";
		case iware::gpu::vendor_t::microsoft:
			return "Microsoft";
		case iware::gpu::vendor_t::qualcomm:
			return "Qualcomm";
		default:
			return "Unknown";
	}
}
