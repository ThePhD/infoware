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


#ifdef WIN32


#include "infoware/detail/winstring.hpp"
#define WIN32_LEAN_AND_MEAN
#include <wbemidl.h>
#include <windows.h>


// Adapted from
// https://gitlab.isb-sib.ch/itopolsk/captain-bol/blob/master/xenobol/src/comutil.h
std::string iware::detail::narrowen_winstring(const wchar_t* str) {
	if(!str)
		return {};

	std::string ret;
	// convert even embeded NUL
	const auto src_len = SysStringLen(const_cast<BSTR>(str));
	if(const auto len = WideCharToMultiByte(CP_ACP, 0, str, src_len, NULL, 0, 0, 0)) {
		ret.resize(len, '\0');
		if(!WideCharToMultiByte(CP_ACP, 0, str, src_len, &ret[0], len, 0, 0))
			ret.clear();
	}
	return ret;
}


#endif
