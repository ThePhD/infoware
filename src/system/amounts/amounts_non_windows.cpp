// SPDX-License-Identifier: CC0-1.0
// infoware - C++ System information Library


#ifndef _WIN32


#include "infoware/detail/scope.hpp"
#include "infoware/system.hpp"
#include <cstring>
#include <wordexp.h>


// http://man7.org/linux/man-pages/man3/wordexp.3.html
static std::size_t count_expansions(const char* of) noexcept {
	wordexp_t exp{};
	iware::detail::quickscope_wrapper exp_deleter{[&]() { wordfree(&exp); }};

	if(wordexp(of, &exp, 0))
		return 0;

	// If nothing is expanded wordexp() returns the original string
	if(exp.we_wordc == 1 && std::strcmp(exp.we_wordv[0], of) == 0)
		return 0;
	else
		return exp.we_wordc;
}


// http://unix.stackexchange.com/questions/25601/how-do-mouse-events-work-in-linux
// https://www.kernel.org/doc/Documentation/input/input.txt section 3.2.2
std::size_t iware::system::mouse_amount() noexcept {
	return count_expansions("/dev/input/mouse*");
}

// https://www.kernel.org/doc/Documentation/input/input.txt section 3.2.1
// Doesn't specify that and there doesn't seem to be a way to get it
std::size_t iware::system::keyboard_amount() noexcept {
	return 0;
}

std::size_t iware::system::other_HID_amount() noexcept {
	return 0;
}


#endif
