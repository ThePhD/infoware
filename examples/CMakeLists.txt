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

cmake_minimum_required(VERSION 3.15.0)

project(infoware_examples VERSION 1.0.0 LANGUAGES CXX C)

add_executable(infoware_cpu_example gpu.cpp)
add_executable(iware::cpu_example ALIAS infoware_cpu_example)
target_link_libraries(infoware_cpu_example PUBLIC iware::infoware)
if (MSVC)
	target_compile_options(infoware_cpu_example
		PRIVATE /W4 /EHsc "$<$<CONFIG:Debug>:/MDd>"
		"$<$<CONFIG:Release>:/MD>"
		"$<$<CONFIG:RelWithDebInfo>:/MD>"
		"$<$<CONFIG:MinSizeRel>:/MD>")
	target_compile_definitions(infoware_cpu_example
		PRIVATE UNICODE _UNICODE)
else()
	target_compile_options(infoware_cpu_example
		PRIVATE -std=c++11
		-Wno-unknown-warning -Wno-unknown-warning-option
		-Wall -Wpedantic -Werror -pedantic -pedantic-errors -Wno-noexcept-type)
endif()

add_executable(infoware_gpu_example gpu.cpp)
add_executable(iware::gpu_example ALIAS infoware_gpu_example)
target_link_libraries(infoware_gpu_example PUBLIC iware::infoware)
if (MSVC)
	target_compile_options(infoware_gpu_example
		PRIVATE /W4 /EHsc "$<$<CONFIG:Debug>:/MDd>"
		"$<$<CONFIG:Release>:/MD>"
		"$<$<CONFIG:RelWithDebInfo>:/MD>"
		"$<$<CONFIG:MinSizeRel>:/MD>")
	target_compile_definitions(infoware_gpu_example
		PRIVATE UNICODE _UNICODE)
else()
	target_compile_options(infoware_gpu_example
		PRIVATE -std=c++11
		-Wno-unknown-warning -Wno-unknown-warning-option
		-Wall -Wpedantic -Werror -pedantic -pedantic-errors -Wno-noexcept-type)
endif()

add_executable(infoware_system_example system.cpp)
add_executable(iware::system_example ALIAS infoware_system_example)
target_link_libraries(infoware_system_example PUBLIC iware::infoware)
if (MSVC)
	target_compile_options(infoware_system_example
		PRIVATE /W4 /EHsc "$<$<CONFIG:Debug>:/MDd>"
		"$<$<CONFIG:Release>:/MD>"
		"$<$<CONFIG:RelWithDebInfo>:/MD>"
		"$<$<CONFIG:MinSizeRel>:/MD>")
	target_compile_definitions(infoware_system_example
		PRIVATE UNICODE _UNICODE)
else()
	target_compile_options(infoware_system_example
		PRIVATE -std=c++11
		-Wno-unknown-warning -Wno-unknown-warning-option
		-Wall -Wpedantic -Werror -pedantic -pedantic-errors -Wno-noexcept-type)
endif()

if (INFOWARE_TESTS)
	add_test(NAME cpu COMMAND infoware_cpu_example)
	add_test(NAME gpu COMMAND infoware_gpu_example)
	add_test(NAME system COMMAND infoware_system_example)
endif()
