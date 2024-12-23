/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#pragma once

#include <core/types.h>

namespace arc {

template<typename T>
class NumericLimits {
public:
    NODISCARD ALWAYS_INLINE static T min()
    {
        static_assert(false);
        return T();
    }

    NODISCARD ALWAYS_INLINE static T max()
    {
        static_assert(false);
        return T();
    }
};

#define ARC_DECLARE_NUMERIC_LIMITS_FOR_PRIMITIVE_TYPE(type, min_value, max_value) \
    template<>                                                                    \
    class NumericLimits<type> {                                                   \
    public:                                                                       \
        NODISCARD ALWAYS_INLINE static type min()                                 \
        {                                                                         \
            return (min_value);                                                   \
        }                                                                         \
        NODISCARD ALWAYS_INLINE static type max()                                 \
        {                                                                         \
            return (max_value);                                                   \
        }                                                                         \
    }

// clang-format off
ARC_DECLARE_NUMERIC_LIMITS_FOR_PRIMITIVE_TYPE(u8,  0, 0xFF);
ARC_DECLARE_NUMERIC_LIMITS_FOR_PRIMITIVE_TYPE(u16, 0, 0xFFFF);
ARC_DECLARE_NUMERIC_LIMITS_FOR_PRIMITIVE_TYPE(u32, 0, 0xFFFFFFFF);
ARC_DECLARE_NUMERIC_LIMITS_FOR_PRIMITIVE_TYPE(u64, 0, 0xFFFFFFFFFFFFFFFF);
// clang-format on

#undef ARC_DECLARE_NUMERIC_LIMITS_FOR_PRIMITIVE_TYPE

}
