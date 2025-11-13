/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#pragma once

#include <core/types.h>

namespace Arc {

void copy_memory(void* destination_buffer, const void* source_buffer, usize byte_count);
void set_memory(void* destination_buffer, u8 byte_value, usize byte_count);
void zero_memory(void* destination_buffer, usize byte_count);

}
