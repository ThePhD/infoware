// infoware - C++ System information Library
//
// Written in 2016-2019 by nabijaczleweli <nabijaczleweli@gmail.com> and ThePhD <phdofthehouse@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>


#ifndef _WIN32
#ifdef INFOWARE_USE_X11


#include "infoware/detail/error.hpp"
#include "infoware/system.hpp"
#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>
#include <cstdlib>
#include <stdexcept>


namespace iware {
	class Display;
	class Screen;
	class ScreenConfiguration;

}  // namespace iware

XRRScreenConfiguration* init_screen_config(_XDisplay* display, size_t screen_number) {
	Window root                    = RootWindow(display, screen_number);
	XRRScreenConfiguration* config = XRRGetScreenInfo(display, root);
	if(!config) {
		throw std::runtime_error(iware::Error() << "Could not get screen information for screen n° " << screen_number);
	}
	return config;
}

class iware::ScreenConfiguration {
public:
	ScreenConfiguration(_XDisplay* display, size_t screen_number) : display_(display), screen_number_(screen_number) {
		this->config_ = init_screen_config(display_, screen_number_);
	}
	~ScreenConfiguration() {
		if(config_) {
			XRRFreeScreenConfigInfo(config_);
		}
	}

	ScreenConfiguration(const ScreenConfiguration& other) { this->config_ = init_screen_config(other.display_, other.screen_number_); };
	ScreenConfiguration(ScreenConfiguration&& other) {
		this->config_ = other.config_;
		other.config_ = nullptr;
	};

	ScreenConfiguration& operator=(const ScreenConfiguration& other) {
		this->config_ = init_screen_config(other.display_, other.screen_number_);
		return *this;
	}

	double current_rate() const {
		int current_rate = XRRConfigCurrentRate(config_);
		return static_cast<double>(current_rate);
	}

	std::vector<iware::system::config_display_t> config() const {
		std::vector<iware::system::config_display_t> configurations;
		int nsize;
		int nrate;
		short* rates;

		XRRScreenSize* sizes;
		sizes = XRRConfigSizes(config_, &nsize);

		for(int i = 0; i < nsize; i++) {
			const uint32_t width  = static_cast<uint32_t>(sizes[i].width);
			const uint32_t height = static_cast<uint32_t>(sizes[i].height);
			rates                 = XRRConfigRates(config_, i, &nrate);
			std::vector<double> refresh_rates(nrate);
			for(int j = 0; j < nrate; j++) {
				refresh_rates.at(j) = rates[j];
			}
			configurations.emplace_back(iware::system::config_display_t{width, height, refresh_rates});
		}
		return configurations;
	}

private:
	XRRScreenConfiguration* config_ = nullptr;
	_XDisplay* display_;
	size_t screen_number_;
};

class iware::Screen {
public:
	Screen(_XDisplay* display, size_t screen_number) : display_(display), screen_number_(screen_number), configuration_(display, screen_number) {}
	~Screen() {}
	uint32_t current_width() const { return static_cast<uint32_t>(DisplayWidth(display_, screen_number_)); };
	uint32_t current_height() const { return static_cast<uint32_t>(DisplayHeight(display_, screen_number_)); };
	uint32_t current_dpi() const {
		// Ratio width(mm)/width(pixels) converted in dots/inch (25.4 millimeters per inch)
		return static_cast<uint32_t>(25.4 * DisplayWidthMM(display_, screen_number_) / current_width());
	};
	uint32_t current_depth() const { return static_cast<uint32_t>(DefaultDepth(display_, screen_number_)); };

	double refresh_rate() const { return configuration_.current_rate(); }
	std::vector<iware::system::config_display_t> available_configurations() const { return configuration_.config(); }

	// Reference on it's parent
	_XDisplay* display_;
	size_t screen_number_;
	ScreenConfiguration configuration_;
};


_XDisplay* init_display() {
	const char* display_name = std::getenv("DISPLAY");
	_XDisplay* display       = XOpenDisplay(display_name);
	if(!display) {
		throw std::runtime_error(iware::Error() << "Could not get display " << display_name);
	}
	return display;
};

class iware::Display {
public:
	Display() {
		display_             = init_display();
		size_t screen_count_ = ScreenCount(display_);
		for(size_t i = 0; i < screen_count_; ++i) {
			screens_.emplace_back(Screen(display_, i));
		}
	}
	~Display() {
		screens_.clear();
		if(display_) {
			XCloseDisplay(display_);
		}
	}
	Display(const Display& other) {
		this->display_ = init_display();
		this->screens_ = other.screens_;
	}
	Display(Display&& other) {
		this->display_ = other.display_;
		this->screens_ = other.screens_;
	}
	Display& operator=(Display other) {
		this->display_ = init_display();
		this->screens_ = other.screens_;
		return *this;
	}

	size_t screen_count() const { return screens_.size(); }
	const std::vector<iware::Screen>& screens() const { return screens_; }


	/// Returns the current display configuration (width, height) for each screen
	std::vector<iware::system::display_t> infos() const;
	/// Returns all available display configuration (width, height) for each screen
	std::vector<std::vector<iware::system::config_display_t>> available_configurations() const;

private:
	_XDisplay* display_;
	std::vector<iware::Screen> screens_;
};


std::vector<iware::system::display_t> iware::Display::infos() const {
	std::vector<iware::system::display_t> infos;
	for(auto& screen : screens()) {
		const uint32_t width  = screen.current_width();
		const uint32_t height = screen.current_height();
		const uint32_t dpi    = screen.current_dpi();
		const uint32_t bpp    = screen.current_depth();
		double refresh_rate   = screen.refresh_rate();
		infos.emplace_back(iware::system::display_t{width, height, dpi, bpp, refresh_rate});
	}
	return infos;
}
std::vector<std::vector<iware::system::config_display_t>> iware::Display::available_configurations() const {
	std::vector<std::vector<iware::system::config_display_t>> configurations;
	for(auto& screen : screens()) {
		configurations.emplace_back(screen.available_configurations());
	}
	return configurations;
}


std::vector<iware::system::display_t> iware::system::displays() {
	iware::Display display;
	return display.infos();
}

std::vector<std::vector<iware::system::config_display_t>> iware::system::available_configurations() {
	iware::Display display;
	return display.available_configurations();
}


#endif
#endif
