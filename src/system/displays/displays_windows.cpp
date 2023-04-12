// SPDX-License-Identifier: CC0-1.0
// infoware - C++ System information Library


#ifdef _WIN32


#include "infoware/detail/scope.hpp"
#include "infoware/system.hpp"
#include <algorithm>
#include <cstdlib>
#include <utility>
#define WIN32_LEAN_AND_MEAN
#include <ShellScalingApi.h>
#include <windows.h>


typedef DPI_AWARENESS_CONTEXT (*STDAC)(DPI_AWARENESS_CONTEXT);
static STDAC set_dpi_awareness = nullptr;

// https://learn.microsoft.com/en-us/windows/win32/hidpi/setting-the-default-dpi-awareness-for-a-process
// <  Windows 8.1       : GetDeviceCaps(hdc, LOGPIXELSX)
// >= Windows 8.1       : GetDpiForMonitor(, MDT_EFFECTIVE_DPI, ,) with SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE), but can lead to
//                        unexpected application behavior.
// >= Windows 10, 1607  : GetDpiForMonitor(, MDT_EFFECTIVE_DPI, ,) with SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2) in
//                        an irrelevant thread
static UINT retrieve_dpi_for_monitor(HMONITOR monitor) {
	UINT dpi = 96, _;  // default value without scaling

	// >= windows 10.0.14393, 1607
	if(set_dpi_awareness) {
		auto PRE_DAC = set_dpi_awareness(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
		iware::detail::quickscope_wrapper dpi_awareness_resetter{[&]() { set_dpi_awareness(PRE_DAC); }};

		// DPI_AWARENESS_CONTEXT_UNAWARE                 : 96 because the app is unaware of any other scale factors.
		// DPI_AWARENESS_CONTEXT_SYSTEM_AWARE            : A value set to the system DPI because the app assumes all applications use the system DPI.
		// DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE(_V2)  : The actual DPI value set by the user for that display.
		if(::GetDpiForMonitor(monitor, MDT_EFFECTIVE_DPI, &dpi, &_) == S_OK) {  // Minimum supported clien: Windows 8.1
			return dpi;
		}
	}

	// < windows 10.0.14393, 1607
	// system (primary monitor) or 96 DPI depending the global DPI avareness setting.
	auto hdc = ::GetDC(nullptr);
	dpi      = ::GetDeviceCaps(hdc, LOGPIXELSX);
	::ReleaseDC(nullptr, hdc);

	return dpi;
}

std::vector<iware::system::display_t> iware::system::displays() {
	std::vector<iware::system::display_t> ret;

	DPI_AWARENESS_CONTEXT PRE_DAC = nullptr;
	HMODULE user32_dll            = nullptr;
	// >= windows 10.0.14393, 1607
	auto ver = iware::system::kernel_info();
	if(ver.major >= 10 && ver.patch >= 14393) {
		// for preventing lower version windows link error
		user32_dll        = ::LoadLibrary(L"user32.dll");
		set_dpi_awareness = reinterpret_cast<STDAC>(::GetProcAddress(user32_dll, "SetThreadDpiAwarenessContext"));

		// we will set thread dpi awareness to block the influence of global dpi awareness settings, 
		// such as Qt, SetProcessDPIAware, SetProcessDpiAwarenessContext.
		if(set_dpi_awareness != nullptr) {
			PRE_DAC = set_dpi_awareness(DPI_AWARENESS_CONTEXT_UNAWARE);  // make GetMonitorInfo to get virtual resolution for computing the scale factor
		}
	}

	// https://learn.microsoft.com/en-us/windows/win32/gdi/multiple-display-monitors
	//
	// The bounding rectangle of all the monitors is the `virtual screen`.
	// The `desktop` covers the virtual screen instead of a single monitor.
	// The `primary monitor` contains the origin (0,0) for compatibility.
	//
	// Each `physical display` is represented by a monitor handle of type HMONITOR.
	// A valid HMONITOR is guaranteed to be non-NULL.
	// A physical display has the same HMONITOR as long as it is part of the desktop.
	//
	// To enumerate all the devices in the virtual screen, use EnumDisplayMonitors.
	::EnumDisplayMonitors(
	    nullptr, nullptr,
	    [](auto monitor, auto, auto, auto userdata) {
		    auto& ret = *reinterpret_cast<std::vector<iware::system::display_t>*>(userdata);

		    MONITORINFOEX info{};
		    info.cbSize = sizeof(MONITORINFOEX);
		    // GetMonitorInfo will be affected by DPI_AWARENESS
		    // DPI_AWARENESS_CONTEXT_UNAWARE                : virtual resolution
		    // DPI_AWARENESS_CONTEXT_SYSTEM_AWARE           : virtual resolution
		    // DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE(_V2) : physical resolution
		    if(::GetMonitorInfo(monitor, &info)) {
			    DEVMODE settings = {};
			    settings.dmSize   = sizeof(DEVMODE);

			    if(::EnumDisplaySettings(info.szDevice, ENUM_CURRENT_SETTINGS, &settings)) {  // not affected by DPI_AWARENESS
				    ret.push_back(
				        {settings.dmPelsWidth, settings.dmPelsHeight, retrieve_dpi_for_monitor(monitor), settings.dmBitsPerPel,
				         static_cast<double>(settings.dmDisplayFrequency), static_cast<std::int32_t>(settings.dmPosition.x),
				         static_cast<std::int32_t>(settings.dmPosition.y), static_cast<double>(settings.dmPelsWidth) / (info.rcMonitor.right - info.rcMonitor.left),
				         (settings.dmPosition.x == 0 && settings.dmPosition.y == 0), static_cast<iware::system::orientation_t>(0x01 << settings.dmDisplayOrientation)});
			    }
		    }

		    return TRUE;
	    },
	    reinterpret_cast<LPARAM>(&ret));

	if(set_dpi_awareness) {
		set_dpi_awareness(PRE_DAC);
		set_dpi_awareness = nullptr;
	}

	if(user32_dll) {
		::FreeLibrary(user32_dll);
	}

	return std::move(ret);
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
