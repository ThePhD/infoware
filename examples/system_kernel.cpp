// infoware - C++ System information Library
//
// Written in 2016 by nabijaczleweli <nabijaczleweli@gmail.com> and ThePhD <phdofthehouse@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>


#include "infoware/system.hpp"
#include <iostream>


int main() {
	const auto kernel_info = iware::system::kernel_info();

	switch(kernel_info.variant) {
		case iware::system::kernel_t::windows_nt:
			std::cout << "Windows NT";
			break;
		case iware::system::kernel_t::linux:
			std::cout << "Linux";
			break;
		case iware::system::kernel_t::darwin:
			std::cout << "OS X";
			break;
		case iware::system::kernel_t::unknown:
			std::cout << "Unknown OS";
			break;
	}

	std::cout << " version " << kernel_info.major << '.' << kernel_info.minor << '.' << kernel_info.patch << " buildnum " << kernel_info.build_number << '\n';
}
