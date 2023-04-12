// SPDX-License-Identifier: CC0-1.0
// infoware - C++ System information Library


#pragma once


#if defined(__i386__) || defined(__x86_64__) || defined(_M_X64) || defined(_M_IX86)
#define INFOWARE_X86 1
#endif /* x86 */

#if defined(__arm64__) || defined(_M_ARM64)
#define INFOWARE_ARM 1
#endif /* ARM */
