// SPDX-License-Identifier: CC0-1.0
// infoware - C++ System information Library


#pragma once

#if defined _MSC_VER
	#if defined INFOWARE_DLL
		#if defined INFOWARE_BUILDING
			#define INFOWARE_API_LINKAGE __declspec(dllexport)
			#define INFOWARE_API_LINKAGE_INTERNAL
		#else
			#define INFOWARE_API_LINKAGE __declspec(dllimport)
			#define INFOWARE_API_LINKAGE_INTERNAL
		#endif // FICAPI_BUILD - Building the Library vs. Using the Library
	#else
		#define INFOWARE_API_LINKAGE
		#define INFOWARE_API_LINKAGE_INTERNAL
	#endif // Building a DLL vs. Static Library
#else  // g++ / clang++
	#define INFOWARE_API_LINKAGE __attribute__((visibility("default")))
	#define INFOWARE_API_LINKAGE_INTERNAL __attribute__((visibility("hidden")))
#endif // MSVC vs. other shared object / dll attributes
