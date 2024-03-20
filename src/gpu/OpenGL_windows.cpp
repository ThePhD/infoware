// SPDX-License-Identifier: CC0-1.0
// infoware - C++ System information Library


#ifndef INFOWARE_USE_VULKAN
#ifndef INFOWARE_USE_D3D
#ifndef INFOWARE_USE_OPENCL
#ifdef _WIN32
#ifdef INFOWARE_USE_OPENGL

#include "infoware/gpu.hpp"


std::vector<iware::gpu::device_properties_t> iware::gpu::device_properties() {
#warning No OpenGL Win32 implementation
	return {};
}


#endif
#endif
#endif
#endif
#endif
