// SPDX-License-Identifier: CC0-1.0
// infoware - C++ System information Library


#ifndef INFOWARE_USE_VULKAN
#ifndef INFOWARE_USE_D3D
#ifndef INFOWARE_USE_OPENCL
#ifndef _WIN32
#ifndef __APPLE__
#ifdef INFOWARE_USE_OPENGL

#include "infoware/detail/scope.hpp"
#include "infoware/gpu.hpp"

#include <EGL/egl.h>
#include <GL/gl.h>
#include <algorithm>
#include <cstring>


static iware::gpu::vendor_t vendor_from_name(std::string name) {
	std::transform(name.begin(), name.end(), name.begin(), ::toupper);

	if(name.find("NVIDIA") != std::string::npos)
		return iware::gpu::vendor_t::nvidia;
	else if(name.find("AMD") != std::string::npos || name.find("ATI") != std::string::npos || name.find("ADVANCED MICRO DEVICES") != std::string::npos)
		return iware::gpu::vendor_t::amd;
	else if(name.find("INTEL") != std::string::npos)
		return iware::gpu::vendor_t::intel;
	else if(name.find("MICROSOFT") != std::string::npos)
		return iware::gpu::vendor_t::microsoft;
	else if(name.find("QUALCOMM") != std::string::npos)
		return iware::gpu::vendor_t::qualcomm;
	else
		return iware::gpu::vendor_t::unknown;
}

std::vector<iware::gpu::device_properties_t> iware::gpu::device_properties() {
	EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	eglInitialize(display, nullptr, nullptr);
	iware::detail::quickscope_wrapper display_deleter{[&]() { eglTerminate(display); }};

	// Choose an EGL Config
	static const EGLint config_attributes[] = {EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT, EGL_NONE};
	EGLConfig config;
	EGLint numConfigs{};
	if(!eglChooseConfig(display, config_attributes, &config, 1, &numConfigs) || !numConfigs)
		return {};

	// Create an EGL Surface
	static const EGLint pbuffer_attributes[] = {EGL_WIDTH, 1, EGL_HEIGHT, 1, EGL_NONE};
	EGLSurface surface                       = eglCreatePbufferSurface(display, config, pbuffer_attributes);
	if(surface == EGL_NO_SURFACE)
		return {};
	iware::detail::quickscope_wrapper surface_deleter{[&]() { eglDestroySurface(display, surface); }};

	static const EGLint context_attributes[] = {EGL_CONTEXT_MAJOR_VERSION, 2, EGL_CONTEXT_MINOR_VERSION, 0, EGL_NONE};
	EGLContext context                       = eglCreateContext(display, config, EGL_NO_CONTEXT, context_attributes);
	if(context == EGL_NO_CONTEXT)
		return {};
	iware::detail::quickscope_wrapper context_deleter{[&]() { eglDestroyContext(display, context); }};

	eglMakeCurrent(display, surface, surface, context);
	iware::detail::quickscope_wrapper context_restorer{[&]() { eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT); }};


	auto vendor          = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
	std::string renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));

	iware::gpu::vendor_t vendor_type = vendor_from_name(vendor);
	if(vendor_type == iware::gpu::vendor_t::unknown)
		vendor_type = vendor_from_name(renderer);

	// Check if context has extension for meminfo, we can access the dedicated vram using GL_NVX_gpu_memory_info
	// which is supported using mesa on AMD and NVIDIA devices, but not on Intel (that is fine, they do not have dedicated VRAM)
	// Funnily enough this won't work on NVIDIA devices at all because NVIDIA does not support EGL higher than OpenGL ES 2.0, this extension
	// is available in OpenGL 2.0 but not in ES
	// In the future there should be a fallback for GLX context especially for NVIDIA devices
	bool has_NVX_mem_info = false;
	if(auto extensions = reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS)))
		has_NVX_mem_info = std::strstr(extensions, "GL_NVX_gpu_memory_info");

	GLint total_memory = 0;
	if(has_NVX_mem_info)
		glGetIntegerv(GL_GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX, &total_memory);


	// We can't get the cache size or the max frequency, so we just set them to 0
	return {device_properties_t{
	    std::move(vendor_type),
	    std::move(renderer),
	    static_cast<std::size_t>(total_memory),
	    0,
	    0,
	}};
}


#endif
#endif
#endif
#endif
#endif
#endif
