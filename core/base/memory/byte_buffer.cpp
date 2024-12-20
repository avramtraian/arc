/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#include <core/base/memory/byte_buffer.h>
#include <core/base/memory/memory_operations.h>

namespace arc {

ByteBuffer ByteBuffer::allocate(usize in_byte_count)
{
    ByteBuffer byte_buffer;
    byte_buffer.allocate_new(in_byte_count);
    return byte_buffer;
}

ByteBuffer ByteBuffer::copy(const ByteBuffer& source_buffer)
{
    ByteBuffer byte_buffer = ByteBuffer::allocate(source_buffer.byte_count());
    copy_memory(byte_buffer.m_bytes, source_buffer.bytes(), byte_buffer.m_byte_count);
    return byte_buffer;
}

ByteBuffer::ByteBuffer()
    : m_bytes(nullptr)
    , m_byte_count(0)
{}

ByteBuffer::~ByteBuffer()
{
    free();
}

ByteBuffer::ByteBuffer(ByteBuffer&& other) noexcept
    : m_bytes(other.m_bytes)
    , m_byte_count(other.m_byte_count)
{
    other.m_bytes = nullptr;
    other.m_byte_count = 0;
}

ByteBuffer& ByteBuffer::operator=(ByteBuffer&& other) noexcept
{
    // Handle self-assignment case.
    if (this == &other)
        return *this;

    free();
    m_bytes = other.m_bytes;
    m_byte_count = other.m_byte_count;
    other.m_bytes = nullptr;
    other.m_byte_count = 0;
    return *this;
}

void ByteBuffer::free()
{
    // Requesting a zero-sized memory block is assumed to yield nullptr. This ensures
    // consistent behaviour across all platforms.
    if (m_byte_count > 0) {
        ::operator delete(m_bytes);
    }

    m_bytes = nullptr;
    m_byte_count = 0;
}

void ByteBuffer::allocate_new(usize in_byte_count)
{
    free();

    // Requesting a zero-sized memory block is assumed to yield nullptr. This ensures
    // consistent behaviour across all platforms.
    if (in_byte_count == 0)
        return;

    m_byte_count = in_byte_count;
    m_bytes = static_cast<ReadWriteBytes>(::operator new(m_byte_count));
}

void ByteBuffer::ensure_byte_count(usize in_byte_count)
{
    if (m_byte_count >= in_byte_count)
        return;

    const ReadWriteBytes new_bytes = static_cast<ReadWriteBytes>(::operator new(in_byte_count));
    copy_memory(new_bytes, m_bytes, m_byte_count);
    ::operator delete(m_bytes);

    m_bytes = new_bytes;
    m_byte_count = in_byte_count;
}

void ByteBuffer::ensure_expansion_byte_count(usize expansion_byte_count)
{
    const usize in_byte_count = m_byte_count + expansion_byte_count;
    ensure_byte_count(in_byte_count);
}

}
