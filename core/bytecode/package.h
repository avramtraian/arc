/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#pragma once

#include <core/base/containers/vector.h>
#include <core/base/memory/byte_buffer.h>

namespace arc {

class Package {
public:
    ByteBuffer byte_buffer;
    usize byte_count;
};

}
