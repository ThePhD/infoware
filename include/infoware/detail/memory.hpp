// SPDX-License-Identifier: CC0-1.0
// infoware - C++ System information Library


#pragma once


namespace iware {
	namespace detail {
		struct release_deleter {
			template <typename T>
			void operator()(T* p) const {
				p->Release();
			}
		};
	}  // namespace detail
}  // namespace iware
