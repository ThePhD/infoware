
#include "infoware/detail/error.hpp"
#include "infoware/system.hpp"
#include <stdexcept>

#include <ApplicationServices/ApplicationServices.h>


// This value determines the maximum number of displays that can be returned.
static const uint32_t MAX_DISPLAYS = 32;


std::vector<iware::system::display_t> iware::system::displays() {
	std::vector<iware::system::display_t> output;


	CGDirectDisplayID displays_id[MAX_DISPLAYS];
	uint32_t num_displays;
	if(CGGetActiveDisplayList(MAX_DISPLAYS, displays_id, &num_displays) != kCGErrorSuccess) {
		throw std::runtime_error(iware::Error() << "Could not get active display list");
	}

	for(size_t i = 0; i < num_displays; ++i) {
		const uint32_t width  = CGDisplayPixelsWide(displays_id[i]);
		const uint32_t height = CGDisplayPixelsHigh(displays_id[i]);

		CGSize display_size_mm  = CGDisplayScreenSize(displays_id[i]);
		const uint32_t width_mm = display_size_mm.width;

		// Ratio width(mm)/width(pixels) converted in dots/inch (25.4 millimeters per inch)
		const uint32_t dpi = 25.4 * width_mm / width;

		CGDisplayModeRef mode_ref = CGDisplayCopyDisplayMode(displays_id[i]);

		CFStringRef str = CGDisplayModeCopyPixelEncoding(mode_ref);
		// A string in the form --------RRRRRRRRGGGGGGGGBBBBBBBB
		std::string pixel_encoding(CFStringGetCStringPtr(str, kCFStringEncodingUTF8));
		CFRelease(str);

		// Count the number of occurences of R,G,B pixels
		const uint32_t bpp = std::count(pixel_encoding.begin(), pixel_encoding.end(), 'R') + std::count(pixel_encoding.begin(), pixel_encoding.end(), 'G') +
		                     std::count(pixel_encoding.begin(), pixel_encoding.end(), 'B');


		// The refresh rate, in hertz, of the specified display mode for a CRT display. Some displays may not use conventional video vertical and horizontal
		// sweep in painting the screen; for these displays, the return value is 0.
		// Returns 0 for Retina display tested
		double refresh_rate = CGDisplayModeGetRefreshRate(mode_ref);

		CGDisplayModeRelease(mode_ref);
		output.emplace_back(iware::system::display_t{width, height, dpi, bpp, refresh_rate});
	}


	return output;
}