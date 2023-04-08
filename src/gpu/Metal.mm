// infoware - C++ System information Library
//
// Written in 2016-2020 by nabijaczleweli <nabijaczleweli@gmail.com> and ThePhD <phdofthehouse@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>


#if __APPLE__ && !defined(INFOWARE_USE_OPENCL)


#include "infoware/gpu.hpp"
#import <Metal/Metal.h>


std::vector<iware::gpu::device_properties_t> iware::gpu::device_properties() {
	NSArray<id<MTLDevice>>* devices = MTLCopyAllDevices();
	std::vector<iware::gpu::device_properties_t> results;

	for(id<MTLDevice> device in devices) {
		const char* name = [device.name UTF8String];

		// there is not cache_size and max_frequency in MTLDevice
		results.push_back(device_properties_t{
		    vendor_t::apple, std::string(name, device.name.length), device.recommendedMaxWorkingSetSize,
		    0,    // OpenCL returns 0 on M2
		    1000  // OpenCL returns 1000 on M2
		});
	}
	return results;
}


#endif /* __APPLE__ && !defined (INFOWARE_USE_OPENCL) */
