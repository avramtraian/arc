/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#pragma once

//========================================================================================================================================//
//---------------------------------------------------------- COMPILER DETECTION ----------------------------------------------------------//
//========================================================================================================================================//

#ifdef __clang__
    #define ARC_COMPILER_CLANG 1
#else
    #ifdef __GNUC__
        #define ARC_COMPILER_GCC 1
    #endif // __GNUC__
    #ifdef _MSC_BUILD
        #define ARC_COMPILER_MSVC 1
    #endif // _MSC_BUILD
#endif // __clang__

#ifndef ARC_COMPILER_CLANG
    #define ARC_COMPILER_CLANG 0
#endif // ARC_COMPILER_CLANG

#ifndef ARC_COMPILER_GCC
    #define ARC_COMPILER_GCC 0
#endif // ARC_COMPILER_GCC

#ifndef ARC_COMPILER_MSVC
    #define ARC_COMPILER_MSVC 0
#endif // ARC_COMPILER_MSVC

#if !ARC_COMPILER_CLANG && !ARC_COMPILER_GCC && !ARC_COMPILER_MSVC
    #error Unknown or unsupported compiler!
#endif // Any compiler.

#define ARC_COMPILER_COUNT (ARC_COMPILER_CLANG + ARC_COMPILER_MSVC + ARC_COMPILER_GCC)
#if (ARC_COMPILER_COUNT > 1)
    #error Multiple compilers were detected!
#endif // There are multiple compiler macros set to 1.

//========================================================================================================================================//
//------------------------------------------------------- PLATFORM OS DETECTION ----------------------------------------------------------//
//========================================================================================================================================//

#if defined(_WIN32)
    #define ARC_PLATFORM_WINDOWS 1
#endif // defined(_WIN32)

#if defined(__linux__)
    #define ARC_PLATFORM_LINUX 1
#endif // defined(__linux__)

#ifndef ARC_PLATFORM_WINDOWS
    #define ARC_PLATFORM_WINDOWS 0
#endif // ARC_PLATFORM_WINDOWS

#ifndef ARC_PLATFORM_LINUX
    #define ARC_PLATFORM_LINUX 0
#endif // ARC_PLATFORM_LINUX

#ifndef ARC_PLATFORM_MACOS
    #define ARC_PLATFORM_MACOS 0
#endif // ARC_PLATFORM_MACOS

#if !ARC_PLATFORM_WINDOWS && !ARC_PLATFORM_LINUX && !ARC_PLATFORM_MACOS
    #error Unknown or unsupported platform operating system!
#endif // Any supported platform.

#define ARC_PLATFORM_COUNT (ARC_PLATFORM_WINDOWS + ARC_PLATFORM_LINUX + ARC_PLATFORM_MACOS)
#if (ARC_PLATFORM_COUNT > 1)
    #error Multiple platform operating systems were detected!
#endif // There are multiple platform operating system macros set to 1.

//========================================================================================================================================//
//---------------------------------------------------- PLATFORM ARCHITECTURE DETECTION ---------------------------------------------------//
//========================================================================================================================================//

#if ARC_COMPILER_MSVC
// NOTE: Check the architecture macros defined by the MSVC compiler.

    #if defined(_M_IX86)
        #define ARC_PLATFORM_ARCHITECTURE_X32 1
    #endif // defined(_M_IX86)
    #if defined(_M_X64) || defined(_M_AMD64)
        #define ARC_PLATFORM_ARCHITECTURE_X64 1
    #endif // defined(_M_X64) || defined(_M_AMD64)
    #if defined(_M_ARM)
        #define ARC_PLATFORM_ARCHITECTURE_ARM32 1
    #endif // defined(_M_ARM)
    #if defined(_M_ARM64)
        #define ARC_PLATFORM_ARCHITECTURE_ARM64 1
    #endif // defined(_M_ARM64)

#elif ARC_COMPILER_CLANG || ARC_COMPILER_GCC
// NOTE: Check the architecture macros defined by both Clang and GCC.

    #if defined(__i386__)
        #define ARC_PLATFORM_ARCHITECTURE_X32 1
    #endif // defined(__i386__)
    #if defined(__x86_64__) || defined(__amd64__)
        #define ARC_PLATFORM_ARCHITECTURE_X64 1
    #endif // defined(__x86_64__) || defined(__amd64__)
    #if defined(__arm__)
        #define ARC_PLATFORM_ARCHITECTURE_ARM32 1
    #endif // defined(__arm__)
    #if defined(__aarch64__)
        #define ARC_PLATFORM_ARCHITECTURE_ARM64 1
    #endif // defined(__aarch64__)

#endif // Supported compilers.

#ifndef ARC_PLATFORM_ARCHITECTURE_X32
    #define ARC_PLATFORM_ARCHITECTURE_X32 0
#endif // ARC_PLATFORM_ARCHITECTURE_X32

#ifndef ARC_PLATFORM_ARCHITECTURE_X64
    #define ARC_PLATFORM_ARCHITECTURE_X64 0
#endif // ARC_PLATFORM_ARCHITECTURE_X64

#ifndef ARC_PLATFORM_ARCHITECTURE_ARM32
    #define ARC_PLATFORM_ARCHITECTURE_ARM32 0
#endif // ARC_PLATFORM_ARCHITECTURE_ARM32

#ifndef ARC_PLATFORM_ARCHITECTURE_ARM64
    #define ARC_PLATFORM_ARCHITECTURE_ARM64 0
#endif // ARC_PLATFORM_ARCHITECTURE_ARM64

#if !ARC_PLATFORM_ARCHITECTURE_X32 && !ARC_PLATFORM_ARCHITECTURE_X64 && !ARC_PLATFORM_ARCHITECTURE_ARM32 && !ARC_PLATFORM_ARCHITECTURE_ARM64
    #error Unnkown or unsupported platform architecture!
#endif // Any supported architecture.

#define ARC_PLATFORM_ARCHITECTURE_COUNT \
    (ARC_PLATFORM_ARCHITECTURE_X32 + ARC_PLATFORM_ARCHITECTURE_X64 + ARC_PLATFORM_ARCHITECTURE_ARM32 + ARC_PLATFORM_ARCHITECTURE_ARM64)
#if (ARC_PLATFORM_ARCHITECTURE_COUNT > 1)
    #error Multiple platform architectures were detected!
#endif // There are multiple platform architecture macros set to 1.

//========================================================================================================================================//
//--------------------------------------------------------- CONFIGURATION MACROS ---------------------------------------------------------//
//========================================================================================================================================//

#ifndef ARC_CONFIGURATION_DEBUG
    #define ARC_CONFIGURATION_DEBUG 0
#endif // ARC_CONFIGURATION_DEBUG

#ifndef ARC_CONFIGURATION_RELEASE
    #define ARC_CONFIGURATION_RELEASE 0
#endif // ARC_CONFIGURATION_RELEASE

#if !ARC_CONFIGURATION_DEBUG && !ARC_CONFIGURATION_RELEASE
    #error Unknown or unsupported build configuration!
#endif // Any supported configuration.

#define ARC_CONFIGURATION_COUNT (ARC_CONFIGURATION_DEBUG + ARC_CONFIGURATION_RELEASE)
#if (ARC_CONFIGURATION_COUNT > 1)
    #error Multiple build configurations were detected!
#endif // There are multiple build configuration macros set to 1.

//========================================================================================================================================//
//------------------------------------------------------------ UTILITY MACROS ------------------------------------------------------------//
//========================================================================================================================================//

#define NODISCARD    [[nodiscard]]
#define MAYBE_UNUSED [[maybe_unused]]
#define LIKELY       [[likely]]
#define UNLIKELY     [[unlikely]]

#if ARC_COMPILER_CLANG || ARC_COMPILER_GCC
    #define ALWAYS_INLINE           __attribute__((always_inline)) inline
    #define ARC_FUNCTION            __PRETTY_FUNCTION__
    #define ARC_PLATFORM_DEBUGBREAK __builtin_trap()
    #define ARC_NORETURN            __attribute__((noreturn))
    #define ARC_UNREACHABLE         __builtin_unreachable()
#elif ARC_COMPILER_MSVC
    #define ALWAYS_INLINE           __forceinline
    #define ARC_FUNCTION            __FUNCSIG__
    #define ARC_PLATFORM_DEBUGBREAK __debugbreak()
    #define ARC_NORETURN            __declspec(noreturn)
    #define ARC_UNREACHABLE         __assume(0)
#endif

#define ARC_IMPL_STRINGIFY(x)      #x
#define ARC_IMPL_CONCATENATE(x, y) x##y

#define ARC_STRINGIFY(x)      ARC_IMPL_STRINGIFY(x)
#define ARC_CONCATENATE(x, y) ARC_IMPL_CONCATENATE(x, y)
