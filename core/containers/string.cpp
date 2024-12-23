/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#include <core/containers/string.h>
#include <core/memory/memory_operations.h>

namespace arc {

String::String()
    : m_byte_count(1)
{
    m_inline_buffer[0] = 0;
}

String::~String()
{
    clear();
}

String::String(const String& other)
    : m_byte_count(other.m_byte_count)
{
    if (is_stored_inline()) {
        copy_memory(m_inline_buffer, other.m_inline_buffer, m_byte_count);
    }
    else {
        m_heap_buffer = other.m_heap_buffer;
        m_heap_buffer->reference_count++;
    }
}

String::String(String&& other) noexcept
    : m_byte_count(other.m_byte_count)
{
    if (is_stored_inline()) {
        copy_memory(m_inline_buffer, other.m_inline_buffer, m_byte_count);
    }
    else {
        m_heap_buffer = other.m_heap_buffer;
        other.m_heap_buffer = nullptr;
    }

    other.m_byte_count = 1;
    zero_memory(other.m_inline_buffer, inline_capacity);
}

String::String(StringView view)
    : m_byte_count(view.byte_count() + 1)
{
    char* destination_buffer = m_inline_buffer;

    if (is_stored_on_heap()) {
        m_heap_buffer = allocate_memory(m_byte_count);
        destination_buffer = m_heap_buffer->characters;
    }

    copy_memory(destination_buffer, view.characters(), view.byte_count());
    destination_buffer[m_byte_count - 1] = '\0';
}

String& String::operator=(const String& other)
{
    // Handle self-assignment case.
    if (this == &other)
        return *this;

    clear();
    m_byte_count = other.m_byte_count;

    if (is_stored_inline()) {
        copy_memory(m_inline_buffer, other.m_inline_buffer, m_byte_count);
    }
    else {
        m_heap_buffer = other.m_heap_buffer;
        m_heap_buffer->reference_count++;
    }

    return *this;
}

String& String::operator=(String&& other) noexcept
{
    // Handle self-assignment case.
    if (this == &other)
        return *this;

    clear();
    m_byte_count = other.m_byte_count;

    if (is_stored_inline()) {
        copy_memory(m_inline_buffer, other.m_inline_buffer, m_byte_count);
    }
    else {
        m_heap_buffer = other.m_heap_buffer;
        other.m_heap_buffer = nullptr;
    }

    other.m_byte_count = 1;
    zero_memory(other.m_inline_buffer, inline_capacity);

    return *this;
}

String& String::operator=(StringView view)
{
    clear();
    m_byte_count = view.byte_count() + 1;

    char* destination_buffer = m_inline_buffer;
    if (is_stored_on_heap()) {
        m_heap_buffer = allocate_memory(m_byte_count);
        destination_buffer = m_heap_buffer->characters;
    }

    copy_memory(destination_buffer, view.characters(), view.byte_count());
    destination_buffer[m_byte_count - 1] = '\0';

    return *this;
}

void String::clear()
{
    if (is_stored_on_heap()) {
        ARC_ASSERT(m_heap_buffer->reference_count > 0);
        m_heap_buffer->reference_count--;
        if (m_heap_buffer->reference_count == 0) {
            // Free the heap buffer if no other string references it.
            free_memory(m_heap_buffer, m_byte_count);
        }
    }

    m_byte_count = 1;
    zero_memory(m_inline_buffer, inline_capacity);
}

String::HeapBuffer* String::allocate_memory(usize in_byte_count)
{
    ARC_ASSERT(in_byte_count > inline_capacity);
    const usize allocation_size = sizeof(HeapBuffer) + in_byte_count;
    void* memory_block = ::operator new(allocation_size);
    HeapBuffer* heap_buffer = new (memory_block) HeapBuffer();
    heap_buffer->reference_count = 1;
    return heap_buffer;
}

void String::free_memory(HeapBuffer* heap_buffer, usize in_byte_count)
{
    ARC_ASSERT(in_byte_count > inline_capacity);
    MAYBE_UNUSED const usize allocation_size = sizeof(HeapBuffer) + in_byte_count;
    ::operator delete(heap_buffer);
}

}
