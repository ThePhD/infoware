// SPDX-License-Identifier: CC0-1.0
// infoware - C++ System information Library


#ifndef _WIN32
#ifndef __APPLE__
#ifdef INFOWARE_USE_X11


#include "infoware/detail/scope.hpp"
#include "infoware/system.hpp"
#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>
#include <cstdlib>


template <class T, class F>
static std::vector<T> enumerate_screens(F&& cbk) {
	const auto display_name = std::getenv("DISPLAY");
	if(!display_name)
		return {};

	auto display = XOpenDisplay(display_name);
	if(!display)
		return {};
	iware::detail::quickscope_wrapper display_deleter{[&]() { XCloseDisplay(display); }};

	const auto screen_count = ScreenCount(display);
	std::vector<T> ret;
	ret.reserve(screen_count);

	for(auto i = 0; i < screen_count; ++i) {
		const auto root = RootWindow(display, i);
		auto config     = XRRGetScreenInfo(display, root);
		if(!config)
			continue;
		iware::detail::quickscope_wrapper config_deleter{[&]() { XRRFreeScreenConfigInfo(config); }};

		ret.emplace_back(std::move(cbk(display, i, config)));
	}

	return ret;
}


std::vector<iware::system::display_t> iware::system::displays() {
	return enumerate_screens<iware::system::display_t>([](auto display, auto screen_number, auto screen_config) {
		const std::uint32_t width = DisplayWidth(display, screen_number);
		return iware::system::display_t{
		    width,
		    static_cast<std::uint32_t>(DisplayHeight(display, screen_number)),
		    // 25.4 millimeters per inch
		    static_cast<std::uint32_t>(25.4 * DisplayWidthMM(display, screen_number) / width),
		    static_cast<std::uint32_t>(DefaultDepth(display, screen_number)),
		    static_cast<double>(XRRConfigCurrentRate(screen_config)),
		};
	});
}

std::vector<std::vector<iware::system::display_config_t>> iware::system::available_display_configurations() {
	return enumerate_screens<std::vector<iware::system::display_config_t>>([](auto, auto, auto screen_config) {
		int size_count;
		const auto sizes = XRRConfigSizes(screen_config, &size_count);

		std::vector<iware::system::display_config_t> configs;
		configs.reserve(size_count);

		for(auto i = 0; i < size_count; i++) {
			int rate_count;
			const auto rates = XRRConfigRates(screen_config, i, &rate_count);

			configs.emplace_back(iware::system::display_config_t{
			    static_cast<std::uint32_t>(sizes[i].width), static_cast<std::uint32_t>(sizes[i].height), {rates, rates + rate_count}});
		}

		return configs;
	});
}


#endif
#endif
#endif
