/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#pragma once

#include <core/containers/span.h>
#include <core/types.h>

namespace arc {

class ByteBuffer {
    ARC_MAKE_NONCOPYABLE(ByteBuffer);

public:
    NODISCARD static ByteBuffer allocate(usize in_byte_count);
    NODISCARD static ByteBuffer copy(const ByteBuffer& source_buffer);

public:
    ByteBuffer();
    ~ByteBuffer();

    ByteBuffer(ByteBuffer&& other) noexcept;
    ByteBuffer& operator=(ByteBuffer&& other) noexcept;

public:
    NODISCARD ALWAYS_INLINE ReadWriteBytes bytes() { return m_bytes; }
    NODISCARD ALWAYS_INLINE ReadonlyBytes bytes() const { return m_bytes; }

    NODISCARD ALWAYS_INLINE usize byte_count() const { return m_byte_count; }
    NODISCARD ALWAYS_INLINE bool is_empty() const { return m_byte_count == 0; }

    NODISCARD ALWAYS_INLINE ReadWriteByteSpan byte_span() { return ReadWriteByteSpan(m_bytes, m_byte_count); }
    NODISCARD ALWAYS_INLINE ReadonlyByteSpan byte_span() const { return ReadonlyByteSpan(m_bytes, m_byte_count); }

    NODISCARD ALWAYS_INLINE ReadWriteByteSpan read_write_byte_span()
    {
        return ReadWriteByteSpan(m_bytes, m_byte_count);
    }

    NODISCARD ALWAYS_INLINE WriteonlyByteSpan writeonly_byte_span() { return WriteonlyByteSpan(m_bytes, m_byte_count); }

    NODISCARD ALWAYS_INLINE ReadonlyByteSpan readonly_byte_span() const
    {
        return ReadonlyByteSpan(m_bytes, m_byte_count);
    }

public:
    void free();
    void allocate_new(usize in_byte_count);

    void ensure_byte_count(usize in_byte_count);
    void ensure_expansion_byte_count(usize expansion_byte_count);

private:
    ReadWriteBytes m_bytes;
    usize m_byte_count;
};

}
