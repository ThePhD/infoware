// infoware - C++ System information Library
//
// Written in 2017 by nabijaczleweli <nabijaczleweli@gmail.com> and ThePhD <phdofthehouse@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>


#pragma once


#include <cstdint>
#include <string>
#include <vector>


namespace iware {
	namespace gpu {
		enum class vendor_t {
			intel,
			amd,
			nvidia,
			microsoft,
			qualcomm,
			unknown,
		};

		struct device_properties_t {
			vendor_t vendor;
			std::string name;
			std::size_t memory_size;
			std::size_t cache_size;
		};


		/// Returns all GPU's properties.
		std::vector<device_properties_t> device_properties();
	}
}
