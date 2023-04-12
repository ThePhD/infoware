// SPDX-License-Identifier: CC0-1.0
// infoware - C++ System information Library


#ifndef _WIN32
#ifndef __APPLE__
#ifndef INFOWARE_USE_X11


#include "infoware/system.hpp"


std::vector<iware::system::display_t> iware::system::displays() {
	return {};
}

std::vector<std::vector<iware::system::display_config_t>> iware::system::available_display_configurations() {
	return {};
}


#endif
#endif
#endif
