#include "infoware/platform.hpp"
#include "infoware/gpu.hpp"
#import <Metal/Metal.h>
#include <vector>

#if INFOWARE_APPLE && !defined (INFOWARE_USE_OPENCL)
std::vector<iware::gpu::device_properties_t> iware::gpu::device_properties() {
      NSArray<id<MTLDevice>>* devices = MTLCopyAllDevices();
      std::vector<iware::gpu::device_properties_t> results{};
      
      for (id<MTLDevice> device in devices) {
            const char* name = [device.name UTF8String];

            // there is not cache_size and max_frequency in MTLDevice
            results.push_back(device_properties_t {
                  vendor_t::apple,
                  std::string(name, device.name.length),
                  device.recommendedMaxWorkingSetSize,
                  0, // OpenCL returns 0 on M2
                  1000 // OpenCL returns 1000 on M2
            });
      }
	return results;
}
#endif /* INFOWARE_APPLE && !defined (INFOWARE_USE_OPENCL) */
