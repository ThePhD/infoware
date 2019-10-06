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


#pragma once

#include <sstream>
namespace iware {

	class Error {
	public:
		Error() {}
		~Error() { stream_ << '\n'; }

		operator std::string() const { return stream_.str(); }

		template <typename Type>
		Error& operator<<(const Type& value) {
			stream_ << value;
			return *this;
		}

	private:
		std::stringstream stream_;
	};
}  // namespace iware