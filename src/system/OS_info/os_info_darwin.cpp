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


#ifdef __APPLE__


#include "infoware/system.hpp"
#include <cstdint>
#include <cstring>
#include <iostream>
#include <sys/sysctl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <utility>
#include <vector>


std::vector<char> get_ctl(const char* name, int top, int bottom) {
	int mib[]{top, bottom};
	std::size_t len{};
	std::cerr << name << ' ' << sysctl(mib, sizeof(mib) / sizeof(*mib), nullptr, &len, nullptr, 0);

	std::vector<char> ret(len);
	std::cerr << ' ' << sysctl(mib, sizeof(mib) / sizeof(*mib), ret.data(), &len, nullptr, 0) << ' ';
	return ret;
}

template <class T>
std::pair<bool, T> ctl_int(const std::vector<char>& data) {
	if(data.size() != sizeof(T))
		return {false, {}};

	T out;
	std::memcpy(&out, data.data(), sizeof(out));
	return {true, out};
}

void get_ctl_str(const char* name, int top, int bottom) {
	const auto data = get_ctl(name, top, bottom);

	if(data.empty())
		std::cerr << "!!!\n";
	else
		std::cerr << data.data() << '\n';
}

void get_ctl_int(const char* name, int top, int bottom) {
	const auto data = get_ctl(name, top, bottom);

	const auto out_16 = ctl_int<std::uint16_t>(data);
	if(out_16.first) {
		std::cerr << out_16.second << '\n';
		return;
	}

	const auto out_32 = ctl_int<std::uint32_t>(data);
	if(out_32.first) {
		std::cerr << out_32.second << '\n';
		return;
	}

	const auto out_64 = ctl_int<std::uint64_t>(data);
	if(out_64.first) {
		std::cerr << out_64.second << '\n';
		return;
	}

	std::cerr << "!!!\n";
}

void get_ctl_clockinfo(const char* name, int top, int bottom) {
	const auto data = get_ctl(name, top, bottom);

	const auto out = ctl_int<clockinfo>(data);
	if(out.first) {
		std::cerr << "hz=" << out.second.hz << " tick=" << out.second.tick << " stathz=" << out.second.stathz << " profhz=" << out.second.profhz << '\n';
		return;
	}

	std::cerr << "!!!\n";
}

iware::system::OS_info_t iware::system::OS_info() {
#ifdef HW_MACHINE
	get_ctl_str("HW_MACHINE", CTL_HW, HW_MACHINE);
#endif
#ifdef HW_MODEL
	get_ctl_str("HW_MODEL", CTL_HW, HW_MODEL);
#endif
#ifdef HW_MACHINE_ARCH
	get_ctl_str("HW_MACHINE_ARCH", CTL_HW, HW_MACHINE_ARCH);
#endif
#ifdef HW_NCPU
	get_ctl_int("HW_NCPU", CTL_HW, HW_NCPU);
#endif
#ifdef HW_BYTEORDER
	get_ctl_int("HW_BYTEORDER", CTL_HW, HW_BYTEORDER);
#endif
#ifdef HW_PHYSMEM
	get_ctl_int("HW_PHYSMEM", CTL_HW, HW_PHYSMEM);
#endif
#ifdef HW_USERMEM
	get_ctl_int("HW_USERMEM", CTL_HW, HW_USERMEM);
#endif
#ifdef HW_PAGESIZE
	get_ctl_int("HW_PAGESIZE", CTL_HW, HW_PAGESIZE);
#endif
#ifdef HW_FLOATINGPT
	get_ctl_int("HW_FLOATINGPT", CTL_HW, HW_FLOATINGPT);
#endif
#ifdef HW_REALMEM
	get_ctl_int("HW_REALMEM", CTL_HW, HW_REALMEM);
#endif

	std::cerr << "\n\n";

#ifdef KERN_BOOTFILE
	get_ctl_str("KERN_BOOTFILE", CTL_KERN, KERN_BOOTFILE);
#endif
#ifdef KERN_HOSTUUID
	get_ctl_str("KERN_HOSTUUID", CTL_KERN, KERN_HOSTUUID);
#endif
#ifdef KERN_HOSTNAME
	get_ctl_str("KERN_HOSTNAME", CTL_KERN, KERN_HOSTNAME);
#endif
#ifdef KERN_NISDOMAINNAME
	get_ctl_str("KERN_NISDOMAINNAME", CTL_KERN, KERN_NISDOMAINNAME);
#endif
#ifdef KERN_OSRELEASE
	get_ctl_str("KERN_OSRELEASE", CTL_KERN, KERN_OSRELEASE);
#endif
#ifdef KERN_OSTYPE
	get_ctl_str("KERN_OSTYPE", CTL_KERN, KERN_OSTYPE);
#endif
#ifdef KERN_VERSION
	get_ctl_str("KERN_VERSION", CTL_KERN, KERN_VERSION);
#endif
#ifdef KERN_ARGMAX
	get_ctl_int("KERN_ARGMAX", CTL_KERN, KERN_ARGMAX);
#endif
#ifdef KERN_HOSTID
	get_ctl_int("KERN_HOSTID", CTL_KERN, KERN_HOSTID);
#endif
#ifdef KERN_JOB_CONTROL
	get_ctl_int("KERN_JOB_CONTROL", CTL_KERN, KERN_JOB_CONTROL);
#endif
#ifdef KERN_MAXFILES
	get_ctl_int("KERN_MAXFILES", CTL_KERN, KERN_MAXFILES);
#endif
#ifdef KERN_MAXFILESPERPROC
	get_ctl_int("KERN_MAXFILESPERPROC", CTL_KERN, KERN_MAXFILESPERPROC);
#endif
#ifdef KERN_MAXPROC
	get_ctl_int("KERN_MAXPROC", CTL_KERN, KERN_MAXPROC);
#endif
#ifdef KERN_MAXPROCPERUID
	get_ctl_int("KERN_MAXPROCPERUID", CTL_KERN, KERN_MAXPROCPERUID);
#endif
#ifdef KERN_MAXVNODES
	get_ctl_int("KERN_MAXVNODES", CTL_KERN, KERN_MAXVNODES);
#endif
#ifdef KERN_NGROUPS
	get_ctl_int("KERN_NGROUPS", CTL_KERN, KERN_NGROUPS);
#endif
#ifdef KERN_OSRELDATE
	get_ctl_int("KERN_OSRELDATE", CTL_KERN, KERN_OSRELDATE);
#endif
#ifdef KERN_OSREV
	get_ctl_int("KERN_OSREV", CTL_KERN, KERN_OSREV);
#endif
#ifdef KERN_POSIX1
	get_ctl_int("KERN_POSIX1", CTL_KERN, KERN_POSIX1);
#endif
#ifdef KERN_QUANTUM
	get_ctl_int("KERN_QUANTUM", CTL_KERN, KERN_QUANTUM);
#endif
#ifdef KERN_SAVED_IDS
	get_ctl_int("KERN_SAVED_IDS", CTL_KERN, KERN_SAVED_IDS);
#endif
#ifdef KERN_SECURELVL
	get_ctl_int("KERN_SECURELVL", CTL_KERN, KERN_SECURELVL);
#endif
#ifdef KERN_UPDATEINTERVAL
	get_ctl_int("KERN_UPDATEINTERVAL", CTL_KERN, KERN_UPDATEINTERVAL);
#endif
#ifdef KERN_CLOCKRATE
	get_ctl_clockinfo("KERN_CLOCKRATE", CTL_KERN, KERN_CLOCKRATE);
#endif

	return {};
}


#endif
