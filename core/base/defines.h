/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#pragma once

#ifdef _WIN32
    #define ARC_PLATFORM_WINDOWS 1
    #ifdef _WIN64
        #define ARC_PLATFORM_ARCH_32_BIT 0
        #define ARC_PLATFORM_ARCH_64_BIT 1
    #else
        #define ARC_PLATFORM_ARCH_32_BIT 1
        #define ARC_PLATFORM_ARCH_64_BIT 0
    #endif // _WIN64
#endif // _WIN32

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
    #error Unknown or unsupported platform!
#endif // Any platform.

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

#define NODISCARD    [[nodiscard]]
#define MAYBE_UNUSED [[maybe_unused]]
#define LIKELY       [[likely]]
#define UNLIKELY     [[unlikely]]

#if ARC_COMPILER_CLANG || ARC_COMPILER_GCC
    #define ALWAYS_INLINE           __attribute__((always_inline)) inline
    #define ARC_FUNCTION            __PRETTY_FUNCTION__
    #define ARC_PLATFORM_DEBUGBREAK __builtin_trap()
#elif ARC_COMPILER_MSVC
    #define ALWAYS_INLINE           __forceinline
    #define ARC_FUNCTION            __FUNCSIG__
    #define ARC_PLATFORM_DEBUGBREAK __debugbreak()
#endif

#define ARC_IMPL_STRINGIFY(x)      #x
#define ARC_IMPL_CONCATENATE(x, y) x##y

#define ARC_STRINGIFY(x)      ARC_IMPL_STRINGIFY(x)
#define ARC_CONCATENATE(x, y) ARC_IMPL_CONCATENATE(x, y)
