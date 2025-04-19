// SPDX-License-Identifier: CC0-1.0
// infoware - C++ System information Library


#ifdef _WIN32


#include "infoware/system.hpp"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <psapi.h>


static timespec FILETIME_to_timespec(const FILETIME& ft) {
	auto ns = 100 * (static_cast<std::uint64_t>(ft.dwHighDateTime) << 32) | static_cast<std::uint64_t>(ft.dwLowDateTime);
	return {static_cast<std::time_t>(ns / 1000'000'000), static_cast<decltype(timespec{}.tv_nsec)>(ns % 1000'000'000)};
}
iware::system::process_stats_t iware::system::process_stats() {
	auto process = GetCurrentProcess();

	FILETIME user_time{}, kernel_time{}, _{};
	GetProcessTimes(process, &_, &_, &kernel_time, &user_time);

	PROCESS_MEMORY_COUNTERS_EX counters{};
	counters.cb = sizeof(counters);
	GetProcessMemoryInfo(process, reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&counters), sizeof(counters));

	IO_COUNTERS io{};
	GetProcessIoCounters(process, &io);

	return {
	    FILETIME_to_timespec(user_time),    //
	    FILETIME_to_timespec(kernel_time),  //

	    counters.PageFaultCount,      //
	    counters.PeakWorkingSetSize,  //

	    io.ReadOperationCount,   //
	    io.WriteOperationCount,  //
	};
}

#endif
