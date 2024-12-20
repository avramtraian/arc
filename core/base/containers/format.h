/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#pragma once

#include <core/base/containers/string.h>
#include <core/base/containers/string_view.h>
#include <core/base/memory/byte_buffer.h>

namespace arc {

class FormatStream {
    ARC_MAKE_NONCOPYABLE(FormatStream);
    ARC_MAKE_NONMOVABLE(FormatStream);

public:
    FormatStream() = default;
    ~FormatStream() = default;

    void push_codepoint(u32 codepoint);
    void push_unsigned_integer(u64 value);
    void push_signed_integer(s64 value);
    void push_floating_point_number(f64 value);
    void push_string(StringView string_view);

    void ensure_byte_count(usize required_byte_count);
    void ensure_push_byte_count(usize push_byte_count);

    NODISCARD StringView formatted_as_string_view() const;

private:
    ByteBuffer m_formatted_buffer;
    usize m_formatted_byte_count { 0 };
};

template<typename T>
class Formatter {
public:
    ALWAYS_INLINE static void format(FormatStream&, const T&)
    {
        // NOTE: You must specialize Formatter<T> in order to format this type!
        static_assert(false);
    }
};

template<typename T>
requires (is_integer<T>)
class Formatter<T> {
public:
    ALWAYS_INLINE static void format(FormatStream& stream, const T& value)
    {
        if constexpr (is_unsigned_integer<T>)
            stream.push_unsigned_integer(static_cast<u64>(value));
        else
            stream.push_signed_integer(static_cast<s64>(value));
    }
};

template<typename T>
requires (is_floating_point<T>)
class Formatter<T> {
public:
    ALWAYS_INLINE static void format(FormatStream& stream, const T& value)
    {
        stream.push_floating_point_number(static_cast<f64>(value));
    }
};

template<>
class Formatter<StringView> {
public:
    static void format(FormatStream& stream, const StringView& value) { stream.push_string(value); }
};

template<>
class Formatter<String> {
public:
    static void format(FormatStream& stream, const String& value) { stream.push_string(StringView(value)); }
};

template<>
class Formatter<char> {
public:
    static void format(FormatStream& stream, const char& value) { stream.push_codepoint(static_cast<u32>(value)); }
};

}
