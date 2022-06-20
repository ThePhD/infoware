// infoware - C++ System information Library
//
// Written in 2016-2020 by nabijaczleweli <nabijaczleweli@gmail.com> and ThePhD <phdofthehouse@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>


#ifdef _WIN32


#include "infoware/detail/scope.hpp"
#include "infoware/system.hpp"
#include <algorithm>
#include <cstdlib>
#include <utility>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>


std::vector<iware::system::display_t> iware::system::displays() {
	const struct bundle {
		HDC desktop_dc;
		std::vector<iware::system::display_t> ret;
	} bundle{GetDC(nullptr), {}};
	iware::detail::quickscope_wrapper desktop_dc_deleter{[&]() { ReleaseDC(nullptr, bundle.desktop_dc); }};

	EnumDisplayMonitors(
	    bundle.desktop_dc, nullptr,
	    [](auto, auto hdc, auto rect, auto userdata) {
		    auto& bundle = *reinterpret_cast<struct bundle*>(userdata);

		    const unsigned int desktop_dpi = GetDeviceCaps(bundle.desktop_dc, LOGPIXELSX);
		    // https://blogs.msdn.microsoft.com/oldnewthing/20101013-00/?p=12543
		    const unsigned int desktop_bpp    = GetDeviceCaps(bundle.desktop_dc, BITSPIXEL) * GetDeviceCaps(bundle.desktop_dc, PLANES);
		    const double desktop_refresh_rate = GetDeviceCaps(bundle.desktop_dc, VREFRESH);


		    // Sometimes returns 0 – fall back to the desktop's globals if so.
		    const unsigned int monitor_dpi    = GetDeviceCaps(hdc, LOGPIXELSX);
		    const unsigned int monitor_bpp    = GetDeviceCaps(hdc, BITSPIXEL) * GetDeviceCaps(hdc, PLANES);
		    const double monitor_refresh_rate = GetDeviceCaps(hdc, VREFRESH);

		    const unsigned int width  = std::abs(rect->right - rect->left);
		    const unsigned int height = std::abs(rect->bottom - rect->top);

		    // See http://stackoverflow.com/a/12654433/2851815 and up for DPI. In short: can't be done too too well, go with best solution.
		    bundle.ret.push_back({width, height, monitor_dpi ? monitor_dpi : desktop_dpi, monitor_bpp ? monitor_bpp : desktop_bpp,
		                          monitor_refresh_rate ? monitor_refresh_rate : desktop_refresh_rate});

		    return TRUE;
	    },
	    reinterpret_cast<LPARAM>(&bundle));

	return std::move(bundle.ret);
}

std::vector<std::vector<iware::system::display_config_t>> iware::system::available_display_configurations() {
	std::vector<std::vector<iware::system::display_config_t>> ret;

	DISPLAY_DEVICE dev;
	dev.cb = sizeof(dev);

	for(DWORD dev_i = 0; EnumDisplayDevices(nullptr, dev_i, &dev, EDD_GET_DEVICE_INTERFACE_NAME); ++dev_i) {
		std::vector<iware::system::display_config_t> configs;
		const auto add_config = [&](auto&& mode) {
			configs.emplace_back(iware::system::display_config_t{mode.dmPelsWidth, mode.dmPelsHeight, {static_cast<double>(mode.dmDisplayFrequency)}});
		};

		DEVMODE mode{};
		for(DWORD mode_i = 0; EnumDisplaySettings(dev.DeviceName, mode_i, &mode); ++mode_i)
			if(configs.empty())
				add_config(mode);
			else {
				auto&& last = configs.back();
				if(last.width == mode.dmPelsWidth && last.height == mode.dmPelsHeight) {
					if(std::find(last.refresh_rates.begin(), last.refresh_rates.end(), mode.dmDisplayFrequency) == last.refresh_rates.end())
						last.refresh_rates.emplace_back(mode.dmDisplayFrequency);
				} else
					add_config(mode);
			}

		if(!configs.empty())
			ret.emplace_back(std::move(configs));
	}

	return ret;
}


#endif
