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


project(infoware VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "lib")


option(INFOWARE_USE_D3D    "Add public, transitive define to infoware to use Direct 3D for GPU Detection." OFF)
option(INFOWARE_USE_OPENGL "Add public, transitive define to infoware to use Open Compute Language (OpenCL) for GPU Detection." OFF)
option(INFOWARE_USE_OPENCL "Add public, transitive define to infoware to use Open Graphics Language (OpenGL) for GPU Detection." OFF)

option(INFOWARE_USE_X11 "Add public, transitive define to infoware to use X11 for Display Detection." OFF)

option(INFOWARE_EXAMPLES "Add infoware examples to the build." OFF)
option(INFOWARE_TESTS    "Input tests for infoware." OFF)


file(GLOB_RECURSE infoware_source_files LIST_DIRECTORIES false src/**/*.cpp)
add_library(infoware ${infoware_source_files})
add_library(iware::infoware ALIAS infoware)

target_include_directories(infoware PUBLIC include)

target_compile_definitions(infoware PRIVATE INFOWARE_INTERNAL_COMPILING)
	
if(MSVC)
	# Only CMake 3.15.0 makes this stuff not necessary, but we have a shitty
	# minimum required version :/
	string(REGEX REPLACE "/W[0-4]" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
	target_compile_options(infoware PRIVATE /EHsc /W4)
	target_compile_definitions(infoware PRIVATE UNICODE _UNICODE)
else()
	target_compile_options(infoware PRIVATE -pedantic -pedantic-errors 
		-Wpedantic -pedantic -Wall -Wextra -Werror 
		-Wno-unknown-warning -Wno-unknown-warning-option
		-pipe -std=c++14)
	if (WIN32)
		target_link_libraries(infoware PRIVATE gdi32 version wbemuuid)
	endif()
endif()

if(WIN32 AND NOT MSVC)
	target_compile_options(infoware PRIVATE -march=native)
endif()

if(WIN32)
	target_link_libraries(infoware PRIVATE gdi32 version Ole32 OleAut32 wbemuuid)
endif()


if(INFOWARE_USE_D3D)
	target_compile_definitions(infoware PUBLIC INFOWARE_USE_D3D)
<<<<<<< HEAD
	target_link_libraries(infoware PUBLIC dxgi)
=======
	if (WIN32)
		target_link_libraries(infoware PUBLIC dxgi.lib)
	endif()
>>>>>>> fix up all derps from unused code
elseif(INFOWARE_USE_OPENGL)
	target_compile_definitions(infoware PUBLIC INFOWARE_USE_OPENGL)

	if(WIN32)
		target_link_libraries(infoware PUBLIC opengl32)
	elseif(DARWIN)
		target_link_libraries(infoware PUBLIC GL)
	else()
		target_link_libraries(infoware PUBLIC "-framework OpenGL")
	endif()
elseif(INFOWARE_USE_OPENCL)
	target_compile_definitions(infoware PUBLIC INFOWARE_USE_OPENCL)
	target_link_libraries(infoware PUBLIC OpenCL)
endif()


if(INFOWARE_USE_X11)
	target_compile_definitions(infoware PUBLIC INFOWARE_USE_X11)
	target_link_libraries(infoware PUBLIC X11)
endif()


if(INFOWARE_EXAMPLES)
	file(GLOB infoware_example_sources LIST_DIRECTORIES false examples/*.cpp)

	foreach(infoware_example_source IN LISTS infoware_example_sources)
		string(REGEX REPLACE "([^/\\]+[/\\])*([^/\\]+)\\.cpp" "\\2" infoware_example_name "${infoware_example_source}")

		add_executable(infoware_${infoware_example_name}_example "${infoware_example_source}")
		add_executable(iware::${infoware_example_name}_example ALIAS infoware_${infoware_example_name}_example)

		target_link_libraries(infoware_${infoware_example_name}_example PUBLIC infoware)

		if(INFOWARE_TESTS)
			add_test(NAME example_${infoware_example_name} COMMAND infoware_${infoware_example_name}_example)
		endif()
	endforeach()
endif()


if(INFOWARE_TESTS)
	enable_testing()
endif()
