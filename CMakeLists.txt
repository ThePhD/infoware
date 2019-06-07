# infoware - C++ System information Library
#
# Written in 2016-2019 by nabijaczleweli <nabijaczleweli@gmail.com> and ThePhD <phdofthehouse@gmail.com>
#
# To the extent possible under law, the author(s) have dedicated all copyright and related
# and neighboring rights to this software to the public domain worldwide. This software is
# distributed without any warranty.
#
# You should have received a copy of the CC0 Public Domain Dedication along with this software.
# If not, see <http://creativecommons.org/publicdomain/zero/1.0/>

cmake_minimum_required(VERSION 3.6.0)

project(infoware VERSION 1.0.0 LANGUAGES CXX C)

option(INFOWARE_USE_D3D "Add public, transitive define to infoware to use Direct 3D for GPU Detection." OFF)
option(INFOWARE_USE_OPENGL "Add public, transitive define to infoware to use Open Compute Language (OpenCL) for GPU Detection." OFF)
option(INFOWARE_USE_OPENCL "Add public, transitive define to infoware to use Open Graphics Language (OpenGL) for GPU Detection." OFF)
option(INFOWARE_USE_X11 "Add public, transitive define to infoware to use X11 for Display Detection." OFF)

option(INFOWARE_EXAMPLES "Add infoware examples to the build." OFF)
option(INFOWARE_TESTS "Input tests for infoware." OFF)

# # TODO: one day in the far, far future
#include(CMakeDependentOption)
#
#option(INFOWARE_DOWNLOAD_PCI_IDS_AND_GENERATE "Download the pci.ids file and generate the pci.generated.cpp file for infoware" OFF)
#CMAKE_DEPENDENT_OPTION(INFOWARE_DOWNLOAD_PCI_IDS_URI "The URI of the pci.ids file." "https://raw.githubusercontent.com/pciutils/pciids/master/pci.ids"
#"INFOWARE_DOWNLOAD_PCI_IDS_AND_GENERATE" "")
#
#add_subdirectory(tools)

if (INFOWARE_EXAMPLES OR INFOWARE_TESTS)
	if (CMAKE_SIZEOF_VOID_P EQUAL 4)
			set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/x86/lib")
			set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/x86/bin")
			set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/x86/bin")
		else()
			set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/x64/lib")
			set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/x64/bin")
			set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/x64/bin")
	endif()
endif()


set(infoware_source_files
	"src/cpu/architecture/architecture_non_windows.cpp"
	"src/cpu/architecture/architecture_windows.cpp"
	"src/cpu/endianness/all.cpp"
	"src/cpu/frequency/frequency_darwin.cpp"
	"src/cpu/frequency/frequency_non_windows_non_darwin.cpp"
	"src/cpu/frequency/frequency_windows.cpp"
	"src/cpu/instruction_set/instruction_set.cpp"
	"src/cpu/instruction_set/instruction_set_darwin.cpp"
	"src/cpu/instruction_set/instruction_set_non_windows_non_darwin.cpp"
	"src/cpu/instruction_set/instruction_set_windows.cpp"
	"src/cpu/quantities_cache/quantities_cache_darwin.cpp"
	"src/cpu/quantities_cache/quantities_cache_non_windows_non_darwin.cpp"
	"src/cpu/quantities_cache/quantities_cache_windows.cpp"
	"src/cpu/vendor_model_name/vendor_id.cpp"
	"src/cpu/vendor_model_name/vendor_model_name_darwin.cpp"
	"src/cpu/vendor_model_name/vendor_model_name_non_windows_non_darwin.cpp"
	"src/cpu/vendor_model_name/vendor_model_name_windows.cpp"
	"src/detail/cpuid_non_windows.cpp"
	"src/detail/cpuid_windows.cpp"
	"src/detail/pci.generated.cpp"
	"src/detail/scope.cpp"
	"src/detail/sysctl_darwin.cpp"
	"src/detail/sysctl_non_darwin.cpp"
	"src/gpu/memory/blank_all.cpp"
	"src/gpu/memory/d3d.cpp"
	"src/gpu/memory/OpenCL.cpp"
	"src/gpu/memory/OpenGL.cpp"
	"src/system/amounts/amounts_non_windows.cpp"
	"src/system/amounts/amounts_windows.cpp"
	"src/system/displays/displays_default_blank.cpp"
	"src/system/displays/displays_windows.cpp"
	"src/system/displays/displays_x11.cpp"
	"src/system/kernel_info/kernel_info_non_windows.cpp"
	"src/system/kernel_info/kernel_info_windows.cpp"
	"src/system/memory/memory_darwin.cpp"
	"src/system/memory/memory_non_windows_non_darwin.cpp"
	"src/system/memory/memory_windows.cpp"
	"src/system/OS_info/os_info_darwin.cpp"
	"src/system/OS_info/os_info_non_windows_non_darwin.cpp"
	"src/system/OS_info/os_info_windows.cpp"
)

add_library(infoware ${infoware_source_files})
add_library(iware::infoware ALIAS infoware)

target_include_directories(infoware PUBLIC include)
target_compile_definitions(infoware PRIVATE INFOWARE_INTERNAL_COMPILING)
if (MSVC)
	target_compile_options(infoware PRIVATE /EHsc /W4)
	target_link_libraries(infoware PRIVATE gdi32 version wbemuuid)
else()
	target_compile_options(infoware PRIVATE -Wno-unknown-warning -Wno-unknown-warning-option
		-Wall -Wpedantic -Werror -pedantic -pedantic-errors -Wno-noexcept-type)
endif()
get_target_property(infoware_library_type infoware TYPE)
if (infoware_library_type STREQUAL "SHARED_LIBRARY")
	target_compile_definitions(infoware PUBLIC INFOWARE_DLL)
endif()

if (INFOWARE_USE_D3D)
	target_compile_definitions(infoware PUBLIC INFOWARE_USE_D3D)
	target_link_libraries(infoware PUBLIC dxgi.lib)
elseif(INFOWARE_USE_OPENGL)
	target_compile_definitions(infoware PUBLIC INFOWARE_USE_OPENGL)
	if (WIN32)
		target_link_libraries(infoware PUBLIC opengl32.lib)
	endif()
elseif(INFOWARE_USE_OPENCL)
	target_compile_definitions(infoware PUBLIC INFOWARE_USE_OPENCL)
endif()

if (INFOWARE_USE_X11)
	target_compile_definitions(infoware PUBLIC INFOWARE_USE_X11)
endif()

if (INFOWARE_TESTS)
	enable_testing()
endif()


if (INFOWARE_EXAMPLES)
	add_subdirectory(examples)
endif()
