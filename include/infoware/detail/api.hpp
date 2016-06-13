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
#include <infoware/detail/compiler.hpp>


#pragma once


namespace iware {

#ifdef INFOWARE_VCXX

#ifdef INFOWARE_INTERNAL_COMPILING

#ifdef INFOWARE_DLL
#define INFOWARE_EXPORT __declspec( dllexport )
#else
#define INFOWARE_EXPORT 
#endif // INFOWARE_DLL 

#else

#ifdef INFOWARE_DLL
#define INFOWARE_EXPORT __declspec( dllimport )
#else
#define INFOWARE_EXPORT 
#endif // INFOWARE_DLL 

#endif // INFOWARE_INTERNAL_COMPILING

#elif defined(INFOWARE_GCC) || defined(INFOWARE_CLANG)

#ifdef INFOWARE_INTERNAL_COMPILING
#define INFOWARE_EXPORT
#else
#define INFOWARE_EXPORT
#endif // INFOWARE_INTERNAL_COMPILING

#endif // WIN32 || APPLE || LINUX 

#define INFOWARE_API INFOWARE_EXPORT

}
