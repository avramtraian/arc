/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#include <core/base/containers/string_view.h>

namespace arc {

StringView StringView::from_utf8(const char* characters, usize byte_count)
{
    StringView view;
    view.m_characters = characters;
    view.m_byte_count = byte_count;
    return view;
}

StringView StringView::from_utf8(const char* null_terminated_characters)
{
    const char* characters = null_terminated_characters;
    while (*characters++) {}
    const usize byte_count_including_null_terminator = characters - null_terminated_characters;

    StringView view;
    view.m_characters = null_terminated_characters;
    // The view is expected to not contain the null-termination character.
    view.m_byte_count = byte_count_including_null_terminator - 1;
    return view;
}

StringView StringView::from_utf8(ReadonlyByteSpan utf8_encoded_bytes)
{
    StringView view;
    view.m_characters = reinterpret_cast<const char*>(utf8_encoded_bytes.elements());
    view.m_byte_count = utf8_encoded_bytes.byte_count();
    return view;
}

void StringView::clear()
{
    m_characters = nullptr;
    m_byte_count = 0;
}

bool StringView::operator==(const StringView& other) const
{
    // If the two strings have different sizes we can be certain that
    // they are not equal, without performing any other operations.
    if (m_byte_count != other.m_byte_count)
        return false;

    for (usize byte_offset = 0; byte_offset < m_byte_count; ++byte_offset) {
        if (m_characters[byte_offset] != other.m_characters[byte_offset])
            return false;
    }

    // All characters match exactly and thus the strings are equal.
    return true;
}

bool StringView::operator!=(const StringView& other) const
{
    const bool are_equal = (*this == other);
    return !are_equal;
}

}
