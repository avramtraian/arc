/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#include <core/containers/format.h>
#include <core/memory/memory_operations.h>

namespace arc {

void FormatStream::push_codepoint(u32 codepoint)
{
    // TODO: Actually implement proper UTF-8 support.
    ensure_push_byte_count(1);
    m_formatted_buffer.bytes()[m_formatted_byte_count++] = codepoint;
}

void FormatStream::push_unsigned_integer(u64 value)
{
    if (value == 0) {
        push_codepoint('0');
        return;
    }

    constexpr usize max_value_bits = 64;
    char formatted_buffer[max_value_bits] = {};
    usize formatted_buffer_byte_count = 0;

    u64 temp_value = value;
    while (temp_value != 0) {
        ++formatted_buffer_byte_count;
        temp_value /= 10;
    }

    temp_value = value;
    usize byte_offset = 1;
    while (temp_value != 0) {
        const char digit = '0' + (temp_value % 10);
        formatted_buffer[formatted_buffer_byte_count - byte_offset] = digit;
        ++byte_offset;
        temp_value /= 10;
    }

    const StringView formatted_string_view = StringView::from_utf8(formatted_buffer, formatted_buffer_byte_count);
    push_string(formatted_string_view);
}

void FormatStream::push_signed_integer(s64 value)
{
    if (value < 0) {
        push_codepoint('-');
        push_unsigned_integer(static_cast<u64>(-value));
    }
    else {
        push_unsigned_integer(static_cast<u64>(value));
    }
}

void FormatStream::push_floating_point_number(f64 value)
{
    const s64 whole_part = static_cast<s64>(value);
    push_signed_integer(whole_part);

    constexpr u8 precision = 4;

    // NOTE: If the precision factor is zero no decimals must be displayed and thus
    //       computing the formatted version of the fractional part is useless.
    if constexpr (precision == 0)
        return;

    // NOTE: The fractional multiplies is basically 10 raised to the power of the precision factor.
    u64 fractional_multiplier = 1;
    for (u8 precision_index = 0; precision_index < precision; ++precision_index)
        fractional_multiplier *= 10;

    u64 fractional_part;
    if (value >= 0.0)
        fractional_part = static_cast<u64>((value - whole_part) * fractional_multiplier);
    else
        fractional_part = static_cast<u64>((whole_part - value) * fractional_multiplier);

    // NOTE: Remove the redundant fractional digits that are zero anyway.
    while (fractional_part >= 10 && fractional_part % 10 == 0)
        fractional_part /= 10;

    push_codepoint('.');
    push_signed_integer(fractional_part);
}

void FormatStream::push_string(StringView string_view)
{
    ensure_push_byte_count(string_view.byte_count());
    copy_memory(m_formatted_buffer.bytes() + m_formatted_byte_count, string_view.characters(),
                string_view.byte_count());
    m_formatted_byte_count += string_view.byte_count();
}

void FormatStream::ensure_byte_count(usize required_byte_count)
{
    m_formatted_buffer.ensure_byte_count(required_byte_count);
}

void FormatStream::ensure_push_byte_count(usize push_byte_count)
{
    const usize required_byte_count = m_formatted_byte_count + push_byte_count;
    ensure_byte_count(required_byte_count);
}

StringView FormatStream::formatted_as_string_view() const
{
    return StringView::from_utf8(ReadonlyByteSpan(m_formatted_buffer.bytes(), m_formatted_byte_count));
}

}
