# infoware - C++ System information Library
#
# Written in 2016-2020 by nabijaczleweli <nabijaczleweli@gmail.com> and ThePhD <phdofthehouse@gmail.com>
#
# To the extent possible under law, the author(s) have dedicated all copyright and related
# and neighboring rights to this software to the public domain worldwide. This software is
# distributed without any warranty.
#
# You should have received a copy of the CC0 Public Domain Dedication along with this software.
# If not, see <http://creativecommons.org/publicdomain/zero/1.0/>

# vcpkg portfile.cmake file for infoware

include(vcpkg_common_functions)

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO ThePhD/infoware
    REF v0.5.3
    SHA512 64a4f0b1027a20bf9c90bf99d638947cb1c0d6bad1b22c8b0403713c5e2c69756435906996b154989f9eaa41e5d55743d7bf5ef51f7bbf8d99eae94d29af1b2c
    HEAD_REF master
)

vcpkg_check_features(
    OUT_FEATURE_OPTIONS FEATURE_OPTIONS
    x11 INFOWARE_USE_X11
    d3d INFOWARE_USE_D3D
    opencl INFOWARE_USE_OPENCL
    opengl INFOWARE_USE_OPENGL
)

# git must be injected, because vcpkg isolates the build 
# from the environment entirely to have reproducible builds
vcpkg_find_acquire_program(GIT)

vcpkg_configure_cmake(
    SOURCE_PATH ${SOURCE_PATH}
    PREFER_NINJA
    OPTIONS
        ${FEATURE_OPTIONS}
        -DINFOWARE_EXAMPLES=OFF
        -DINFOWARE_TESTS=OFF
        -DGIT_EXECUTABLE=${GIT}
        -DGIT_FOUND=true
)

vcpkg_install_cmake()

# make sure the LICENSE file is present as the copyright file
file(INSTALL ${SOURCE_PATH}/LICENSE DESTINATION ${CURRENT_PACKAGES_DIR}/share/infoware RENAME copyright)
# remove things for debug version
# vcpkg also handle share stuff and configuration for debug vs. release,
# so not our responsibility!
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/share")
