/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#pragma once

#include <core/containers/span.h>
#include <core/types.h>

namespace arc {

// Forward declaration.
class String;

class StringView {
public:
    NODISCARD static StringView from_utf8(const char* characters, usize byte_count);
    NODISCARD static StringView from_utf8(const char* null_terminated_characters);
    NODISCARD static StringView from_utf8(ReadonlyByteSpan utf8_encoded_bytes);

public:
    ALWAYS_INLINE constexpr StringView()
        : m_characters(nullptr)
        , m_byte_count(0)
    {}

    ALWAYS_INLINE constexpr StringView(const StringView& other)
        : m_characters(other.m_characters)
        , m_byte_count(other.m_byte_count)
    {}

    ALWAYS_INLINE constexpr StringView& operator=(const StringView& other)
    {
        m_characters = other.m_characters;
        m_byte_count = other.m_byte_count;
        return *this;
    }

    StringView(const String& string);
    StringView& operator=(const String& string);

public:
    NODISCARD ALWAYS_INLINE const char* characters() const { return m_characters; }
    NODISCARD ALWAYS_INLINE usize byte_count() const { return m_byte_count; }

    NODISCARD ALWAYS_INLINE bool is_empty() const { return m_byte_count == 0; }
    NODISCARD ALWAYS_INLINE bool has_characters() const { return m_byte_count > 0; }

public:
    void clear();

    NODISCARD bool operator==(const StringView& other) const;
    NODISCARD bool operator!=(const StringView& other) const;

private:
    const char* m_characters;
    usize m_byte_count;
};

#if ARC_COMPILER_CLANG
    // Temporarily disables the following compiler warning:
    // warning: user-defined literal suffixes not starting with '_' are reserved.
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wuser-defined-literals"
#endif // ARC_COMPILER_CLANG

#if ARC_COMPILER_GCC
    // Temporarily disables the following compiler warning:
    // warning: literal operator suffixes not preceded by '_' are reserved for future standardization.
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wliteral-suffix"
#endif // ARC_COMPILER_CLANG

#if ARC_COMPILER_MSVC
    // Temporarily disables the following compiler warning:
    // 'operator ""sv': literal suffix identifiers that do not start with an underscore are reserved.
    #pragma warning(push)
    #pragma warning(disable : 4455)
#endif // ARC_COMPILER_MSVC

NODISCARD ALWAYS_INLINE StringView operator""sv(const char* literal, usize literal_length)
{
    return StringView::from_utf8(literal, literal_length);
}

#if ARC_COMPILER_CLANG
    #pragma clang diagnostic pop
#endif // ARC_COMPILER_CLANG
#if ARC_COMPILER_GCC
    #pragma GCC diagnostic pop
#endif // ARC_COMPILER_GCC
#if ARC_COMPILER_MSVC
    #pragma warning(pop)
#endif // ARC_COMPILER_MSVC

}
