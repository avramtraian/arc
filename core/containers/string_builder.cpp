/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#include <core/containers/string_builder.h>
#include <core/memory/memory_operations.h>

#define ARC_FORMAT_SPECIFIER_TOKEN_BEGIN '{'
#define ARC_FORMAT_SPECIFIER_TOKEN_END   '}'

namespace Arc {

StringBuilder::StringBuilder()
    : m_byte_count(0)
{}

String StringBuilder::release_string()
{
    const StringView builder_view = StringView::from_utf8(ReadonlyByteSpan(m_characters_buffer.bytes(), m_byte_count));
    return String(builder_view);
}

void StringBuilder::append(StringView string_view)
{
    ensure_append_byte_count(string_view.byte_count());
    copy_memory(m_characters_buffer.bytes() + m_byte_count, string_view.characters(), string_view.byte_count());
    m_byte_count += string_view.byte_count();
}

void StringBuilder::append_indentation(u32 indentation_level)
{
    ensure_append_byte_count(indentation_level * sizeof(' '));
    set_memory(m_characters_buffer.bytes() + m_byte_count, ' ', indentation_level);
    m_byte_count += indentation_level;
}

void StringBuilder::append_newline()
{
    append("\n"sv);
}

void StringBuilder::ensure_append_byte_count(usize append_byte_count)
{
    const usize buffer_required_byte_count = m_byte_count + append_byte_count;
    m_characters_buffer.ensure_byte_count(buffer_required_byte_count);
}

void StringBuilder::consume_until_format_specifier(StringView& cursor)
{
    const char* characters_iterator = cursor.characters();
    usize byte_offset = 0;

    while (byte_offset < cursor.byte_count() && *characters_iterator != ARC_FORMAT_SPECIFIER_TOKEN_BEGIN) {
        ++byte_offset;
        ++characters_iterator;
    }

    ensure_append_byte_count(byte_offset);
    append(StringView::from_utf8(cursor.characters(), byte_offset));

    if (byte_offset < cursor.byte_count())
        cursor = StringView::from_utf8(characters_iterator, cursor.byte_count() - byte_offset);
    else
        cursor.clear();
}

void StringBuilder::consume_format_specifier(StringView& cursor)
{
    ARC_ASSERT(cursor.characters()[0] == ARC_FORMAT_SPECIFIER_TOKEN_BEGIN);

    const char* characters_iterator = cursor.characters();
    usize byte_offset = 0;

    while (byte_offset < cursor.byte_count() && *characters_iterator != ARC_FORMAT_SPECIFIER_TOKEN_END) {
        ++byte_offset;
        ++characters_iterator;
    }

    if (byte_offset == cursor.byte_count()) {
        cursor.clear();
        return;
    }

    ++characters_iterator;
    ++byte_offset;
    cursor = StringView::from_utf8(characters_iterator, cursor.byte_count() - byte_offset);
}

}
