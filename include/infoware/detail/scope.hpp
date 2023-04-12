// SPDX-License-Identifier: CC0-1.0
// infoware - C++ System information Library


#pragma once


#include <functional>


namespace iware {
	namespace detail {
		namespace {
			struct quickscope_wrapper {
				std::function<void()> func;

				quickscope_wrapper(const quickscope_wrapper&) = delete;
				~quickscope_wrapper() {
					if(func)
						func();
				}
			};
		}  // namespace
	}    // namespace detail
}  // namespace iware
