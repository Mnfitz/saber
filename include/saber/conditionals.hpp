/////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2024 Matthew Fitzgerald
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
/////////////////////////////////////////////////////////////////////

#pragma once
#ifndef SABRE_CONDITIONALS_HPP
#define SABRE_CONDITIONALS_HPP

// std
#include <assert.h>
#include <stdexcept>

// "conditionals.hpp"
//
// Provides "conditional compilation" support in the form of a
// set of sabre-defined preprocessor macros usable within the
// sabre source base. It does this by: 1) examining "magic"
// compiler-specific "predefined macros"; then 2) creating a
// compatible set of compiler-independent sabre macros. These
// sabre macro provide compile-time bool expressions for use
// in preprocessor "#if/#elif" directives.
//
// (Note: Always use: "#if/#elif"! Never use: "#ifdef/#if defined()")
// Example usage:
//
//	#include "sabre/conditionals.hpp"
//
//	#if SABRE_CPU(X86)
//		// Intel-specific header file
//		#include "x86_intrin.h"
//
//	#elif SABRE_CPU_ARCH(ARM, 64)
//		// ARM64-specific header file
//		#include "arm_intrinsics.h"
//
//	#else
//		#error "Unsupported cpu"
//	#endif
//
//	struct WindowPosition
//	{
//		bool mIsVisible{};
//
//	#if SABRE_PLATFORM(WIN32)
//		// Use Windows-specific functions/types: POINT/SIZE/UINT
//		POINT mOrigin{};
//		SIZE mSize{};
//		UINT mDpi{96};
//
//	#elif SABRE_PLATFORM(OSX)
//		// Use Mac-specific functions/types: NSPoint/NSSize
//		NSPoint mOrigin{};
//		NSSize mSize{};
//
//	#else
//		#error "Unsupported platform"
//	#endif
//	};

// ------------------------------------------------------------------
#pragma region public SABRE_MACROS

// Sizeof cpu processor architecture (in bits)
// (arch) := (32|64)
#define SABRE_ARCH(arch) (SABRE_PRIVATE_ARCH_##arch(arch))

// Type of cpu processor architecture
// (cpu) := (ARM|X86)
#define SABRE_CPU(cpu) (SABRE_PRIVATE_CPU_##cpu(cpu))

// Tuple of Type/Sizeof cpu processor architecture
// (cpu, arch) = (ARM|X86, 32|64)
#define SABRE_CPU_ARCH(cpu, arch) (SABRE_CPU(cpu) && SABRE_ARCH(arch))

// Target OS type for compiled source
// (platform) := (OSX|IOS|LINUX|WIN32)
#define SABRE_PLATFORM(platform) (SABRE_PRIVATE_PLATFORM_##platform(platform))

// Compiler type being used for compilation
// (compiler) := (CLANG|GCC|MSVC)
#define SABRE_COMPILER(compiler) (SABRE_PRIVATE_COMPILER_##compiler(compiler))

#pragma endregion ()

// ------------------------------------------------------------------
#pragma region private (do not use)! SABRE_MACROS

// TRICKY j3fitz 28apr2024: "Sorta private" sabre macros w/ unused dummy parameter...
// The following #undef's serve no actual purpose other than as a preview-hint
// for humans to understand "which/how-many" macros are expected to be defined
// by this header. Note that these macros are _PRIVATE_ and SHOULD NOT BE USED
// in actual source code. Instead, use the provided public accessor macros.
//
// Q: So, what's tricky about these macros?
// A: When #define'd, they must "accept an unused dummy parameter"!
// This allows for a crude form of "syntax checking" for macros to prevent
// accidental typos when using the public accessor macros.
// For instance:
//
// BAD Definition:
//	#define SABRE_PRIVATE_PLATFORM_WIN32 1 // BAD: Macro without any parameter
//	#define SABRE_PLATFORM(platform) (SABRE_PRIVATE_PLATFORM_##platform)
// Use:
//	#if SABRE_PLATFORM(WIN33) // Oops! Typo->WIN33 instead of WIN32!
//	// BAD: "Silently fails" because undefined macro is same as 0 to preprecessor!
//
// GOOD Definition:
//	#define SABRE_PRIVATE_PLATFORM_WIN32(platform) 1 // GOOD: Macro with unused parameter
//	#define SABRE_PLATFORM(platform) (SABRE_PRIVATE_PLATFORM_##platform(platform))
// Use:
//	#if SABRE_PLATFORM(WIN33) // Hooray! Compile error!
//	// GOOD: Compiler gives error "undefined macro: SABRE_PRIVATE_PLATFORM_WIN33()"

#undef SABRE_PRIVATE_ARCH_32	// 32bits
#undef SABRE_PRIVATE_ARCH_64	// 64bits

#undef SABRE_PRIVATE_CPU_ARM	// ARM
#undef SABRE_PRIVATE_CPU_X86	// Intel/AMD

// #undef SABRE_PRIVATE_PLATFORM_ANDROID // not yet
#undef SABRE_PRIVATE_PLATFORM_IOS
#undef SABRE_PRIVATE_PLATFORM_OSX
#undef SABRE_PRIVATE_PLATFORM_LINUX
#undef SABRE_PRIVATE_PLATFORM_WIN32

#undef SABRE_PRIVATE_COMPILER_CLANG
#undef SABRE_PRIVATE_COMPILER_GCC
#undef SABRE_PRIVATE_COMPILER_MSVC

#pragma endregion ()

#if defined(_MSC_VER) // MSVC-specific "magic" preprocessor symbol announcing "Microsoft"
// ---------------------------------------------------
// Microsoft:
// #if SABRE_PLATFORM(WIN32)...

	#include <winapifamily.h>
	#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP) // exclude UWP apps
		#define SABRE_PRIVATE_PLATFORM_WIN32(platform)	1
	#else
		#define SABRE_PRIVATE_PLATFORM_WIN32(platform)	0
		#error "Unsupported platform (msvc)"
	#endif // WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)

	#define SABRE_PRIVATE_PLATFORM_IOS(platform)	0
	#define SABRE_PRIVATE_PLATFORM_OSX(platform)	0
	#define SABRE_PRIVATE_PLATFORM_LINUX(platform)	0

	#define SABRE_PRIVATE_COMPILER_CLANG(compiler)	0
	#define SABRE_PRIVATE_COMPILER_GCC(compiler)	0
	#define SABRE_PRIVATE_COMPILER_MSVC(compiler)	1

	#if defined(_M_IX86) // x86 32bit
		#define SABRE_PRIVATE_ARCH_32(arch)	1
		#define SABRE_PRIVATE_ARCH_64(arch)	0
		#define SABRE_PRIVATE_CPU_ARM(cpu)	0
		#define SABRE_PRIVATE_CPU_X86(cpu)	1
	#elif defined(_M_X64) // x86 64bit
		#define SABRE_PRIVATE_ARCH_32(arch)	0
		#define SABRE_PRIVATE_ARCH_64(arch)	1
		#define SABRE_PRIVATE_CPU_ARM(cpu)	0
		#define SABRE_PRIVATE_CPU_X86(cpu)	1
	#elif defined(_M_ARM) // arm 32bit
		#define SABRE_PRIVATE_ARCH_32(arch)	1
		#define SABRE_PRIVATE_ARCH_64(arch)	0
		#define SABRE_PRIVATE_CPU_ARM(cpu)	1
		#define SABRE_PRIVATE_CPU_X86(cpu)	0
	#elif defined(_M_ARM64) // arm 64bit
		#define SABRE_PRIVATE_ARCH_32(arch)	0
		#define SABRE_PRIVATE_ARCH_64(arch)	1
		#define SABRE_PRIVATE_CPU_ARM(cpu)	1
		#define SABRE_PRIVATE_CPU_X86(cpu)	0
	#else
		#error "Unsupported architecture (msvc)"
	#endif

	#define SABRE_DEBUG	_DEBUG
	#define SABRE_LOG(expr)
	#define SABRE_THROW(expr)

// REVISIT jfitz 29apr2024: Add MinGW toolset support under windows?
// MinGW: the Linux-derived toolset that is installed along with git-bash.
// Essentially, a GCC toolset, but running under Windows instead of Linux.
// If so, maybe we'll need another case here. Something like:
//
//	#elif defined(__CYGWIN__) && !defined(_WIN32)
//		// Q: New PLATFORM to describe this? Or is Linux sufficient? Investigate!
//		#define SABRE_PRIVATE_PLATFORM_CYGWIN(platform)	1
//		#define SABRE_PRIVATE_PLATFORM_LINUX(platform)	1
//		// etc...

#elif defined(__APPLE__) && defined(__MACH__) // Xcode-specific "magic" preprocessor symbols announcing "Apple"
// ---------------------------------------------------
// Apple:
// #if SABRE_PLATFORM(OSX) || SABRE_PLATFORM(IOS)...

	#include <TargetConditionals.h>
	#if TARGET_OS_MAC // OSX
		#define SABRE_PRIVATE_PLATFORM_IOS(platform)	0
		#define SABRE_PRIVATE_PLATFORM_OSX(platform)	1

		#define SABRE_PRIVATE_ARCH_32(arch)	0 // Never 32bit
		#define SABRE_PRIVATE_ARCH_64(arch)	1 // Always 64bit
		#define SABRE_PRIVATE_CPU_ARM(cpu)	(defined(__aarch64__))
		#define SABRE_PRIVATE_CPU_X86(cpu)	(defined(__x86_64__))

	#elif TARGET_OS_IPHONE // IOS
		#define SABRE_PRIVATE_PLATFORM_IOS(platform)	1
		#define SABRE_PRIVATE_PLATFORM_OSX(platform)	0

		#define SABRE_PRIVATE_ARCH_32(arch)	0 // Never 32bit
		#define SABRE_PRIVATE_ARCH_64(arch)	1 // Always 64bit
		#define SABRE_PRIVATE_CPU_ARM(cpu)	1 // Always ARM
		#define SABRE_PRIVATE_CPU_X86(cpu)	0

	#elif TARGET_IPHONE_SIMULATOR // IOS (when building for simulator)
		#define PLATFORM_NAME "ios" // Apple iOS
		#define SABRE_PRIVATE_PLATFORM_IOS(platform)	1
		#define SABRE_PRIVATE_PLATFORM_OSX(platform)	0

		#define SABRE_PRIVATE_ARCH_32(arch)	0 // Never 32bit
		#define SABRE_PRIVATE_ARCH_64(arch)	1 // Always 64bit
		#define SABRE_PRIVATE_CPU_ARM(cpu)	(defined(__aarch64__))
		#define SABRE_PRIVATE_CPU_X86(cpu)	(defined(__x86_64__))
	#endif

	#define SABRE_PRIVATE_PLATFORM_LINUX(platform)	0
	#define SABRE_PRIVATE_PLATFORM_WIN32(platform)	0

	#define SABRE_PRIVATE_COMPILER_CLANG(compiler)	(defined(__clang__))
	#define SABRE_PRIVATE_COMPILER_GCC(compiler)	(defined(__GNUC__) && !defined(__clang__))
	#define SABRE_PRIVATE_COMPILER_MSVC(compiler)	0

	#define SABRE_DEBUG	(!defined(NDEBUG))
	#define SABRE_LOG(expr)
	#define SABRE_THROW(expr)

#elif defined(__linux__) // GCC-specific "magic" preprocessor symbol announcing "Linux"
// ---------------------------------------------------
// Linux:
// #if SABRE_PLATFORM(LINUX) || SABRE_PLATFORM(ANDROID)...

	#define SABRE_PRIVATE_PLATFORM_LINUX(platform)	1
	// Not yet...
	//#if defined(__ANDROID__)
	//	#define SABRE_PRIVATE_PLATFORM_ANDROID(platform)	1
	//	#define SABRE_PRIVATE_PLATFORM_LINUX(platform)	0
	//#else
	//	#define SABRE_PRIVATE_PLATFORM_ANDROID(platform)	0
	//	#define SABRE_PRIVATE_PLATFORM_LINUX(platform)	1
	//#endif

	#define SABRE_PRIVATE_PLATFORM_IOS(platform)	0
	#define SABRE_PRIVATE_PLATFORM_OSX(platform)	0
	#define SABRE_PRIVATE_PLATFORM_WIN32(platform)	0

	#if defined(__x86_64__)
		#define SABRE_PRIVATE_ARCH_32(arch)	0
		#define SABRE_PRIVATE_ARCH_64(arch)	1
		#define SABRE_PRIVATE_CPU_ARM(cpu)	0
		#define SABRE_PRIVATE_CPU_X86(cpu)	1

	#elif defined(__i386__)
		#define SABRE_PRIVATE_ARCH_32(arch)	1
		#define SABRE_PRIVATE_ARCH_64(arch)	0
		#define SABRE_PRIVATE_CPU_ARM(cpu)	0
		#define SABRE_PRIVATE_CPU_X86(cpu)	1

	#elif defined(__arm__)
		#define SABRE_PRIVATE_ARCH_32(arch)	1
		#define SABRE_PRIVATE_ARCH_64(arch)	0
		#define SABRE_PRIVATE_CPU_ARM(cpu)	1
		#define SABRE_PRIVATE_CPU_X86(cpu)	0

	#elif defined(__aarch64__)
		#define SABRE_PRIVATE_ARCH_32(arch)	0
		#define SABRE_PRIVATE_ARCH_64(arch)	1
		#define SABRE_PRIVATE_CPU_ARM(cpu)	1
		#define SABRE_PRIVATE_CPU_X86(cpu)	0

	#else
		#error "Unsupported architecture"
	#endif

	#define SABRE_PRIVATE_COMPILER_CLANG(compiler)	(defined(__clang__))
	#define SABRE_PRIVATE_COMPILER_GCC(compiler)	(defined(__GNUC__) && !defined(__clang__))
	#define SABRE_PRIVATE_COMPILER_MSVC(compiler)	0

	#define SABRE_DEBUG	(!defined(NDEBUG))
	#define SABRE_LOG(expr)
	#define SABRE_THROW(expr)

#else
#error "Unsupported toolset"
#endif

// Runtime validation macros
// REVISIT j3fitz 28apr2024: Could these be made "inline functions" instead of macros?
// E.g: void sabre::require(bool inCondition); Intead: of SABRE_REQUIRE(condition);

#define SABRE_ASSERT(expr)		{ assert(expr); }
#define SABRE_ENSURE(expr)		{ if (!(expr)) { throw std::runtime_error{"sabre::ensure"}; } }
#define SABRE_REQUIRE(expr)		{ if (!(expr)) { throw std::invalid_argument{"sabre::require"}; } }

#endif // SABRE_CONDITIONALS_HPP
