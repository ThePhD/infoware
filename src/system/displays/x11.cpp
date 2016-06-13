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


#ifndef _WIN32
#ifdef INFOWARE_USE_X11


#include "infoware/system.hpp"
#include <X11/Xlib.h>
#include <cstdlib>


std::vector<iware::system::display_t> iware::system::displays() {
	std::vector<iware::system::display_t> ret;

	if(const auto display_name = getenv("DISPLAY"))
		if(const auto display = XOpenDisplay(display_name))
			for(auto i = 0u; i < display->nscreens; ++i) {
				const auto& screen = display.screens[i];
				// 25.4 millimeters per inch
				ret.emplace_back({screen.width, screen.height, 25.4 * screen.mwidth / screen.width, screen.root_depth});
			}

	return {};
}


#endif
#endif
