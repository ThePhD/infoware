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


#ifdef _WIN32


#include "infoware/system.hpp"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>


std::vector<iware::system::display_t> iware::system::displays() {
	static unsigned int desktop_dpi = GetDeviceCaps(GetDC(nullptr), LOGPIXELSX);


	std::vector<iware::system::display_t> ret;

	EnumDisplayMonitors(
	    nullptr, nullptr,
	    [](auto, auto hdc, auto rect, auto userdata) {
		    // https://msdn.microsoft.com/en-us/library/windows/desktop/dn280510(v=vs.85).aspx
		    // Doesn't work, can't link to shcore
		    //
		    // unsigned int dpi_x;
		    // unsigned int dpi_y;
		    // GetDpiForMonitor(monitor /* arg #1 */, MDT_DEFAULT, &dpi_x, &dpi_y);

		    // Sometimes returns 0 for some reason?
		    // Fall back to the desktop's global DPI if so.
		    unsigned int monitor_dpi = GetDeviceCaps(hdc, LOGPIXELSX);

		    auto& ret = *reinterpret_cast<std::vector<iware::system::display_t>*>(userdata);
		    // See http://stackoverflow.com/a/12654433/2851815 and up for DPI. In short: can't be done too too well, go with best solution.
		    ret.push_back({static_cast<unsigned int>(rect->right), static_cast<unsigned int>(rect->bottom), monitor_dpi ? monitor_dpi : desktop_dpi});

		    return 1;
		  },
	    reinterpret_cast<LPARAM>(&ret));

	return ret;
}


#endif
