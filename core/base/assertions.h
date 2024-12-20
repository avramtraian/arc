/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#pragma once

#include <core/base/types.h>

namespace arc {

enum class AssertionKind : u8 {
    Assert,
    AssertDebug,
    AssertNotReached,
};

void arc_assertion_failed(AssertionKind kind, const char* expression, const char* file, const char* function,
                          u32 line_number);

}

#define ARC_ASSERT(...)                                                                                            \
    if (!(__VA_ARGS__)) {                                                                                          \
        ::arc::arc_assertion_failed(::arc::AssertionKind::Assert, #__VA_ARGS__, __FILE__, ARC_FUNCTION, __LINE__); \
        ARC_PLATFORM_DEBUGBREAK;                                                                                   \
    }

#define ARC_ASSERT_DEBUG(...)                                                                                \
    if (!(__VA_ARGS__)) {                                                                                    \
        ::arc::arc_assertion_failed(::arc::AssertionKind::AssertDebug, #__VA_ARGS__, __FILE__, ARC_FUNCTION, \
                                    __LINE__);                                                               \
        ARC_PLATFORM_DEBUGBREAK;                                                                             \
    }

#define ARC_ASSERT_NOT_REACHED                                                                               \
    {                                                                                                        \
        ::arc::arc_assertion_failed(::arc::AssertionKind::AssertNotReached, nullptr, __FILE__, ARC_FUNCTION, \
                                    __LINE__);                                                               \
        ARC_PLATFORM_DEBUGBREAK;                                                                             \
    }
