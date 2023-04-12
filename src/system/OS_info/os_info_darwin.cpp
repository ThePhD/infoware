// SPDX-License-Identifier: CC0-1.0
// infoware - C++ System information Library


#ifdef __APPLE__


#include "infoware/detail/sysctl.hpp"
#include "infoware/system.hpp"
#include <cstdlib>


iware::system::OS_info_t iware::system::OS_info() {
	auto ctl_osrelease_data = iware::detail::sysctl("kern.osrelease");
	char* osrelease_marker  = &ctl_osrelease_data[0];

	const auto ctl_osrevision_data = iware::detail::sysctl("kern.osrevision");
	unsigned int build_number{};
	if(!ctl_osrevision_data.empty()) {
		const auto osrevision_data = iware::detail::deconstruct_sysctl_int(ctl_osrevision_data);
		if(osrevision_data.first)
			build_number = osrevision_data.second;
	}

	return {iware::detail::sysctl("kern.ostype"),                                                  //
	        iware::detail::sysctl("kern.version"),                                                 //
	        static_cast<unsigned int>(std::strtoul(osrelease_marker, &osrelease_marker, 10)),      //
	        static_cast<unsigned int>(std::strtoul(osrelease_marker + 1, &osrelease_marker, 10)),  //
	        static_cast<unsigned int>(std::strtoul(osrelease_marker + 1, nullptr, 10)),            //
	        build_number};
}


#endif
