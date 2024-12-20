/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#pragma once

#include <core/base/assertions.h>
#include <core/base/containers/string_view.h>
#include <core/base/types.h>

namespace arc {

class String {
public:
    static constexpr usize inline_capacity = sizeof(void*);
    static_assert(inline_capacity > 0);

    struct HeapBuffer {
        u32 reference_count { 0 };
        char characters[];
    };

    friend class StringBuilder;

public:
    String();
    ~String();

    String(const String& other);
    String(String&& other) noexcept;
    String(StringView view);

    String& operator=(const String& other);
    String& operator=(String&& other) noexcept;
    String& operator=(StringView view);

public:
    NODISCARD ALWAYS_INLINE bool is_stored_inline() const { return m_byte_count <= inline_capacity; }
    NODISCARD ALWAYS_INLINE bool is_stored_on_heap() const { return m_byte_count > inline_capacity; }

    NODISCARD ALWAYS_INLINE bool is_empty() const
    {
        ARC_ASSERT(m_byte_count > 0);
        return m_byte_count <= 1;
    }

    NODISCARD ALWAYS_INLINE bool has_characters() const
    {
        ARC_ASSERT(m_byte_count > 0);
        return m_byte_count >= 2;
    }

    NODISCARD ALWAYS_INLINE usize byte_count() const
    {
        ARC_ASSERT(m_byte_count > 0);
        return m_byte_count - 1;
    }

    NODISCARD ALWAYS_INLINE usize byte_count_including_null_terminator() const
    {
        ARC_ASSERT(m_byte_count > 0);
        return m_byte_count;
    }

    NODISCARD ALWAYS_INLINE const char* characters() const
    {
        return is_stored_inline() ? m_inline_buffer : m_heap_buffer->characters;
    }

    NODISCARD ALWAYS_INLINE ReadonlyBytes bytes() const
    {
        return is_stored_inline() ? reinterpret_cast<ReadonlyBytes>(m_inline_buffer)
                                  : reinterpret_cast<ReadonlyBytes>(m_heap_buffer->characters);
    }

public:
    void clear();

private:
    NODISCARD static HeapBuffer* allocate_memory(usize in_byte_count);
    static void free_memory(HeapBuffer* heap_buffer, usize in_byte_count);

private:
    usize m_byte_count;
    union {
        char m_inline_buffer[inline_capacity];
        HeapBuffer* m_heap_buffer;
    };
};

}
