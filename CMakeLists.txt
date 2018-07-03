cmake_minimum_required(VERSION 3.10)

# ----------------
# Infoware library
# ----------------

project(infoware)

set(SOURCES

    include/infoware/cpu.hpp
    include/infoware/gpu.hpp
    include/infoware/infoware.hpp
    include/infoware/system.hpp

    include/infoware/detail/cpuid.hpp
    include/infoware/detail/memory.hpp
    include/infoware/detail/pci.hpp
    include/infoware/detail/scope.hpp

    src/cpu/architecture/architecture_non_windows.cpp
    src/cpu/architecture/architecture_windows.cpp

    src/cpu/endianness/all.cpp

    src/cpu/frequency/frequency_non_windows.cpp
    src/cpu/frequency/frequency_windows.cpp

    src/cpu/instuction_set/instruction_set.cpp
    src/cpu/instuction_set/instruction_set_non_windows.cpp
    src/cpu/instuction_set/instruction_set_windows.cpp

    src/cpu/quantities_cache/quantities_cache_non_windows.cpp
    src/cpu/quantities_cache/quantities_cache_windows.cpp

    src/cpu/vendor_model_name/vendor_id.cpp
    src/cpu/vendor_model_name/vendor_model_name_non_windows.cpp
    src/cpu/vendor_model_name/vendor_model_name_windows.cpp

    src/detail/cpuid.cpp
    src/detail/pci.generated.cpp
    src/detail/scope.cpp

    src/gpu/memory/blank_all.cpp
    src/gpu/memory/d3d.cpp
    src/gpu/memory/OpenCL.cpp
    src/gpu/memory/OpenGL.cpp

    src/system/amounts/amounts_non_windows.cpp
    src/system/amounts/windows.cpp

    src/system/displays/displays_default_blank.cpp
    src/system/displays/displays_windows.cpp
    src/system/displays/displays_x11.cpp

    src/system/kernel_info/kernel_info_non_windows.cpp
    src/system/kernel_info/kernel_info_windows.cpp

    src/system/memory/memory_non_windows.cpp
    src/system/memory/memory_windows.cpp

    src/system/OS_info/os_info_non_windows.cpp
    src/system/OS_info/os_info_windows.cpp
)

add_library(${PROJECT_NAME} STATIC ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# On Windows one needs to link to gdi32, version, Ole32, OleAut32 and wbemuuid
if(WIN32)
    target_link_libraries(${PROJECT_NAME} gdi32 version Ole32 OleAut32 wbemuuid)
endif()

# Options

option(USE_X11    "Use X11 for display detection" OFF)
option(USE_D3D    "Use D3D for GPU detection"     OFF)
option(USE_OPENCL "Use OpenCL for GPU detection"  OFF)
option(USE_OPENGL "Use OpenGL for GPU detection"  OFF)

if(USE_X11)
    add_definitions(-DINFOWARE_USE_X11)
    target_link_libraries(${PROJECT_NAME} X11)
endif()

if(USE_D3D)
    add_definitions(-DINFOWARE_USE_D3D)
    target_link_libraries(${PROJECT_NAME} dxgi)
endif()

if(USE_OPENCL)
    add_definitions(-DINFOWARE_USE_OPENCL)
    target_link_libraries(${PROJECT_NAME} OpenCL)
endif()

if(USE_OPENGL)
    add_definitions(-DINFOWARE_USE_OPENGL)

    if(WIN32)
        set(OPENGL_LIB_LINK_NAME opengl32)
    elseif(APPLE)
        set(OPENGL_LIB_LINK_NAME GL)
    else()
        set(OPENGL_LIB_LINK_NAME OpenGL)
    endif()

    target_link_libraries(${PROJECT_NAME} ${OPENGL_LIB_LINK_NAME})
endif()

# --------
# Examples
# --------

option(BUILD_EXAMPLES "Build examples" ON)

if(BUILD_EXAMPLES)
    foreach(EXAMPLE_NAME cpu;gpu;system)
        add_executable(example_${EXAMPLE_NAME} examples/${EXAMPLE_NAME}.cpp)
        target_link_libraries(example_${EXAMPLE_NAME} infoware)
    endforeach()
endif()
