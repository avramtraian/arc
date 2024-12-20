/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#pragma once

#include <core/base/numeric_limits.h>
#include <core/base/types.h>

namespace arc {

struct JumpAddress {
public:
    NODISCARD ALWAYS_INLINE static JumpAddress invalid()
    {
        JumpAddress invalid_jump_address;
        invalid_jump_address.value = NumericLimits<u64>::max();
        return invalid_jump_address;
    }

public:
    ALWAYS_INLINE JumpAddress()
        : value(NumericLimits<u64>::max())
    {}

    ALWAYS_INLINE explicit JumpAddress(u64 in_value)
        : value(in_value)
    {}

public:
    u64 value;
};

}
