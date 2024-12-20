/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#pragma once

#include <core/base/numeric_limits.h>
#include <core/base/types.h>

namespace arc {

struct Register {
public:
    NODISCARD ALWAYS_INLINE static Register invalid()
    {
        Register invalid_register;
        invalid_register.index = NumericLimits<u8>::max();
        return invalid_register;
    }

public:
    ALWAYS_INLINE Register()
        : index(NumericLimits<u8>::max())
    {}

    ALWAYS_INLINE explicit Register(u8 in_index)
        : index(in_index)
    {}

    NODISCARD ALWAYS_INLINE operator u8() const { return index; }

public:
    u8 index;
};

}
