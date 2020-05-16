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
    REF v0.5.2
    SHA512 ed3b4384c135ebdcead21afc3b5ccb790d7058f5dadfb31a5be52e10078d9140851e2971f3e974a9915dd8a5d2615be3bcee2270baf145b921f46b27c8182cd7
    HEAD_REF master
)

vcpkg_check_features(
  OUT_FEATURE_OPTIONS FEATURE_OPTIONS
  x11 INFOWARE_USE_X11
  d3d INFOWARE_USE_D3D
  opencl INFOWARE_USE_OPENCL
  opengl INFOWARE_USE_OPENGL
)

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
# remove things for debug version of how vcpkg does stuff...?
# they also handle share stuff and configuration for debug vs. release, so not our responsibility
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/share")