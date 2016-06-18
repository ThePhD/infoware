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


#include "infoware/gpu.hpp"
#include <iostream>


int main() {
	for(auto&& props : iware::gpu::device_properties())
		switch(props.vendor) {
			case iware::gpu::vendor_t::intel:
				std::cout << "Intel ";
				break;
			case iware::gpu::vendor_t::AMD:
				std::cout << "AMD ";
				break;
			case iware::gpu::vendor_t::NVidia:
				std::cout << "NVidia ";
				break;
			case iware::gpu::vendor_t::microsoft:
				std::cout << "Microsoft ";
				break;
			case iware::gpu::vendor_t::qualcomm:
				std::cout << "Qualcomm ";
				break;
			case iware::gpu::vendor_t::unknown:
				std::cout << "Unknown ";
				break;
		}
	std::cout << '\n';
}
