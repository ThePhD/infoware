// SPDX-License-Identifier: CC0-1.0
// infoware - C++ System information Library


#ifndef _WIN32


#include "infoware/system.hpp"
#include <sys/resource.h>


iware::system::process_stats_t iware::system::process_stats() {
	struct rusage usage;
	getrusage(RUSAGE_SELF, &usage);
	return {
	    {usage.ru_utime.tv_sec, usage.ru_utime.tv_usec * 1000},  //
	    {usage.ru_stime.tv_sec, usage.ru_stime.tv_usec * 1000},  //

	    static_cast<std::size_t>(usage.ru_minflt + usage.ru_majflt),  //
	    static_cast<std::size_t>(usage.ru_maxrss * 1024),             //

	    static_cast<std::uint64_t>(usage.ru_inblock),  //
	    static_cast<std::uint64_t>(usage.ru_oublock),  //
	};
}


#endif
