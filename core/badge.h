/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#pragma once

#include <core/types.h>

namespace Arc::core {

template<typename T>
class Badge {
    ARC_MAKE_NONCOPYABLE(Badge);
    ARC_MAKE_NONMOVABLE(Badge);

    // NOTE: Because the only available constructor is marked as private, the only typename that
    //       can instantiate this badge is 'T'.
    friend T;

public:
    ~Badge() = default;

private:
    Badge() = default;
};

}

namespace Arc {
using core::Badge;
}
