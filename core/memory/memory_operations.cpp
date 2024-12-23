/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#include <core/memory/memory_operations.h>

namespace arc {

void copy_memory(void* destination_buffer, const void* source_buffer, usize byte_count)
{
    const WriteonlyBytes dst_buffer = static_cast<WriteonlyBytes>(destination_buffer);
    const ReadonlyBytes src_buffer = static_cast<ReadonlyBytes>(source_buffer);

    for (usize byte_offset = 0; byte_offset < byte_count; ++byte_offset) {
        dst_buffer[byte_offset] = src_buffer[byte_offset];
    }
}

void set_memory(void* destination_buffer, u8 byte_value, usize byte_count)
{
    const WriteonlyBytes dst_buffer = static_cast<WriteonlyBytes>(destination_buffer);

    for (usize byte_offset = 0; byte_offset < byte_count; ++byte_offset) {
        dst_buffer[byte_offset] = byte_value;
    }
}

void zero_memory(void* destination_buffer, usize byte_count)
{
    const WriteonlyBytes dst_buffer = static_cast<WriteonlyBytes>(destination_buffer);

    for (usize byte_offset = 0; byte_offset < byte_count; ++byte_offset) {
        dst_buffer[byte_offset] = 0;
    }
}

}
