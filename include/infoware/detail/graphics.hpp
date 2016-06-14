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


#pragma once


#include <infoware/detail/compiler.hpp>
#include <infoware/detail/os.hpp>


namespace iware {

#if defined(INFOWARE_USE_OPENCL) || defined(INFOWARE_USE_OPENGL) || defined(INFOWARE_USE_D3D) 
	// Use user preferences before defining any defaults
#else
	// Otherwise...
#if defined INFOWARE_WIN32
#if defined INFOWARE_MINGW
	// Prefer OpenGL here because MinGW
#define INFOWARE_USE_OPENGL 1
#else
	// Otherwise, VC++ and others do well with D3D
#define INFOWARE_USE_D3D 1
#endif 
#else
// Prefer Open-GL on Non-Windows machines
#define INFOWARE_USE_OPENGL 1
// TODO: possible to detect with a macro if OpenCL is "around"? (Probably not)
#endif // Win32 or Not

#endif
}
