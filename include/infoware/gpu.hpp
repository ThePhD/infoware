// SPDX-License-Identifier: CC0-1.0
// infoware - C++ System information Library


#pragma once


#include <infoware/linkage.hpp>

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
			apple,
			unknown,
		};

		struct device_properties_t {
			vendor_t vendor;
			std::string name;
			std::size_t memory_size;
			std::size_t cache_size;
			std::uint64_t max_frequency;
		};


		/// Returns all GPU's properties.
		INFOWARE_API_LINKAGE std::vector<device_properties_t> device_properties();
	}  // namespace gpu
}  // namespace iware
