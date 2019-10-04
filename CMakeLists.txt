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


find_package(Git)
execute_process(COMMAND ${GIT_EXECUTABLE} describe --tags --abbrev=0 OUTPUT_VARIABLE infoware_version ERROR_QUIET)
string(STRIP "${infoware_version}" infoware_version)
if(infoware_version STREQUAL "")
	set(infoware_version "0.1.0")
else()
	string(REPLACE "v" "" infoware_version "${infoware_version}")
endif()

project(infoware VERSION ${infoware_version} LANGUAGES CXX)


option(INFOWARE_USE_D3D    "Add public, transitive define to infoware to use Direct 3D for GPU detection."                       OFF)
option(INFOWARE_USE_OPENGL "Add public, transitive define to infoware to use Open Graphics Language (OpenGL) for GPU detection." OFF)
option(INFOWARE_USE_OPENCL "Add public, transitive define to infoware to use Open Compute Language (OpenCL) for GPU detection."  OFF)

option(INFOWARE_USE_X11 "Add public, transitive define to infoware to use X11 for display detection." OFF)

option(INFOWARE_EXAMPLES "Add infoware examples to the build." OFF)
option(INFOWARE_TESTS    "Input tests for infoware."           OFF)


if(INFOWARE_USE_D3D)
	if(INFOWARE_USE_OPENCL)
		message(WARNING "INFOWARE_USE_OPENCL specified, but the higher-priority INFOWARE_USE_D3D was specified, too, and will be used instead.")
	elseif(INFOWARE_USE_OPENGL)
		message(WARNING "INFOWARE_USE_OPENGL specified, but the higher-priority INFOWARE_USE_D3D was specified, too, and will be used instead.")
	endif()
endif()

if(INFOWARE_USE_OPENCL)
	if(INFOWARE_USE_OPENGL)
		message(WARNING "INFOWARE_USE_OPENGL specified, but the higher-priority INFOWARE_USE_OPENCL was specified, too, and will be used instead.")
	endif()
endif()

if(INFOWARE_USE_X11 AND WIN32)
	message(WARNING "INFOWARE_USE_X11 specified, but compiling for Win32, WinAPI will be used instead.")
endif()


file(GLOB_RECURSE infoware_source_files LIST_DIRECTORIES false src/*.cpp)

set(PRIVATE_HEADERS
	include/infoware/detail/cpuid.hpp
	include/infoware/detail/memory.hpp
	include/infoware/detail/pci.hpp
	include/infoware/detail/scope.hpp
	include/infoware/detail/sysctl.hpp
	include/infoware/detail/winstring.hpp)

set(PUBLIC_HEADERS
	include/infoware/infoware.hpp
	include/infoware/cpu.hpp
	include/infoware/gpu.hpp
	include/infoware/system.hpp
	include/infoware/version.hpp)

add_library(${PROJECT_NAME}
	${infoware_source_files}
	${PUBLIC_HEADERS}
	${PRIVATE_HEADERS}
)

add_library(iware::infoware ALIAS infoware)


target_include_directories(infoware PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")

set_target_properties(infoware PROPERTIES CXX_STANDARD 14
                                          CXX_STANDARD_REQUIRED ON
                                          CXX_EXTENSIONS OFF
										  PUBLIC_HEADER "${PUBLIC_HEADERS}"
                                          RUNTIME_OUTPUT_DIRECTORY "bin"
                                          LIBRARY_OUTPUT_DIRECTORY "bin"
										  ARCHIVE_OUTPUT_DIRECTORY "lib"
										  DEBUG_POSTFIX "d"
                                		  MINSIZEREL_POSTFIX "msr"
                                		  RELWITHDEBINFO_POSTFIX "rwdi")

target_compile_definitions(infoware PRIVATE INFOWARE_VERSION="${infoware_version}")


# Thanks, @griwes
if(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/pciids/pci.ids")
	execute_process(COMMAND ${GIT_EXECUTABLE} pull
	                WORKING_DIRECTORY pciids)
else()
	execute_process(COMMAND ${GIT_EXECUTABLE} clone https://github.com/pciutils/pciids)
endif()

add_executable(infoware_pci_generator tools/pci_generator.cpp)
set_target_properties(infoware_pci_generator PROPERTIES CXX_STANDARD 14
                                                        CXX_STANDARD_REQUIRED ON
                                                        CXX_EXTENSIONS OFF)
add_custom_target(infoware_generate_pcis
                  COMMAND ${CMAKE_COMMAND} -E make_directory infoware_generated/
                  COMMAND $<TARGET_FILE:infoware_pci_generator> "${CMAKE_CURRENT_BINARY_DIR}/pciids/pci.ids" > "infoware_generated/pci_data.hpp"
                  DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/pciids/pci.ids"
                  BYPRODUCTS "infoware_generated/pci_data.hpp")
add_dependencies(infoware infoware_generate_pcis)


if(MSVC)
	# Only CMake 3.15.0 makes this stuff not necessary, but we have a shitty
	# minimum required version :/
	string(REGEX REPLACE "/W[0-4]" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
	target_compile_options(infoware PRIVATE /EHsc /W4)
	target_compile_definitions(infoware PRIVATE UNICODE _UNICODE)
else()
	target_compile_options(infoware PRIVATE -pedantic -Wall -Wextra -pipe)
endif()

if(WIN32 AND NOT MSVC)
	target_compile_options(infoware PRIVATE -march=native)
endif()

if(WIN32)
	target_link_libraries(infoware PRIVATE gdi32 version Ole32 OleAut32 wbemuuid)
endif()


if(INFOWARE_USE_D3D)
	target_compile_definitions(infoware PUBLIC INFOWARE_USE_D3D)
	if (WIN32)
		target_link_libraries(infoware PUBLIC dxgi)
	endif()
elseif(INFOWARE_USE_OPENCL)
	target_compile_definitions(infoware PUBLIC INFOWARE_USE_OPENCL)
	target_link_libraries(infoware PUBLIC OpenCL)
elseif(INFOWARE_USE_OPENGL)
	target_compile_definitions(infoware PUBLIC INFOWARE_USE_OPENGL)

	if(WIN32)
		target_link_libraries(infoware PUBLIC opengl32)
	elseif(DARWIN)
		target_link_libraries(infoware PUBLIC GL)
	else()
		target_link_libraries(infoware PUBLIC "-framework OpenGL")
	endif()
endif()


if(INFOWARE_USE_X11)
	include(FindX11)
	target_compile_definitions(infoware PUBLIC INFOWARE_USE_X11)
	target_include_directories(infoware PUBLIC ${X11_INCLUDE_DIR})
	target_link_libraries(infoware PUBLIC ${X11_LIBRARIES})
endif()


if(INFOWARE_EXAMPLES)
	file(GLOB infoware_example_sources LIST_DIRECTORIES false examples/*.cpp)

	foreach(infoware_example_source IN LISTS infoware_example_sources)
		string(REGEX REPLACE "([^/\\]+[/\\])*([^/\\]+)\\.cpp" "\\2" infoware_example_name "${infoware_example_source}")
		string(REPLACE "/" "" infoware_example_name "${infoware_example_name}")

		add_executable(infoware_${infoware_example_name}_example "${infoware_example_source}")
		add_executable(iware::${infoware_example_name}_example ALIAS infoware_${infoware_example_name}_example)

		target_link_libraries(infoware_${infoware_example_name}_example PUBLIC infoware)

		set_target_properties(infoware_${infoware_example_name}_example PROPERTIES CXX_STANDARD 14
		                                                                           CXX_STANDARD_REQUIRED ON
		                                                                           CXX_EXTENSIONS OFF

																	                                             RUNTIME_OUTPUT_DIRECTORY "bin"
																	                                             LIBRARY_OUTPUT_DIRECTORY "bin"
																	                                             ARCHIVE_OUTPUT_DIRECTORY "lib")

		if(INFOWARE_TESTS)
			add_test(NAME example_${infoware_example_name} COMMAND infoware_${infoware_example_name}_example)
		endif()
	endforeach()
endif()


if(INFOWARE_TESTS)
	enable_testing()
endif()

# If CMAKE_INSTALL_PREFIX is not specified, GNU install directories will be used
include(GNUInstallDirs)

# Specify where library and headers will be installed
install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

target_include_directories(infoware PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# Create infowareConfigVersion package used to specify package version
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/cmake/${PROJECT_NAME}ConfigVersion.cmake"
  VERSION ${VERSION}
  COMPATIBILITY AnyNewerVersion
)

# Install infowareConfigVersion file
install(
  FILES
  ${CMAKE_CURRENT_BINARY_DIR}/cmake/${PROJECT_NAME}ConfigVersion.cmake
  DESTINATION ${CMAKE_INSTALL_PREFIX}/share/${PROJECT_NAME}
  COMPONENT devel
)

# Specifying config file that will be used to find a library using find_package().
export(TARGETS ${PROJECT_NAME}
	FILE ${PROJECT_NAME}Config.cmake
)

# Install config file
install (EXPORT ${PROJECT_NAME}Targets
	FILE ${PROJECT_NAME}Config.cmake
	DESTINATION ${CMAKE_INSTALL_PREFIX}/share/${PROJECT_NAME}
)

