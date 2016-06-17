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
	const auto OS_info = iware::system::OS_info();
	std::cout << OS_info.full_name << " (" << OS_info.name << ") version " << OS_info.major << '.' << OS_info.minor << '.' << OS_info.patch << " buildnum "
	          << OS_info.build_number << '\n';
}
