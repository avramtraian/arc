/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#pragma once

#include <core/base/containers/format.h>
#include <core/base/containers/string.h>
#include <core/base/memory/byte_buffer.h>

namespace arc {

class StringBuilder {
    ARC_MAKE_NONCOPYABLE(StringBuilder);
    ARC_MAKE_NONMOVABLE(StringBuilder);

public:
    template<typename... Args>
    NODISCARD ALWAYS_INLINE static String formatted(StringView format, Args&&... parameters)
    {
        StringBuilder builder;
        builder.formatted_implementation(format, forward<Args>(parameters)...);
        return builder.release_string();
    }

public:
    StringBuilder();
    ~StringBuilder() = default;

    NODISCARD String release_string();

public:
    void append(StringView string_view);

    template<typename... Args>
    void append(StringView format, Args&&... parameters)
    {
        formatted_implementation(format, forward<Args>(parameters)...);
    }

    template<typename T>
    ALWAYS_INLINE void append_formatted(const T& value)
    {
        FormatStream stream;
        Formatter<T>::format(stream, value);
        append(stream.formatted_as_string_view());
    }

private:
    void ensure_append_byte_count(usize append_byte_count);

    template<typename CurrentParameter, typename... Args>
    ALWAYS_INLINE void formatted_implementation(StringView& format, const CurrentParameter& current_parameter,
                                                Args&&... parameters)
    {
        consume_until_format_specifier(format);
        // NOTE: The number of parameters passed to the `formatted` function doesn't match the
        //       number of format specifiers located in the format string.
        ARC_ASSERT(format.has_characters());
        consume_format_specifier(format);
        append_formatted(current_parameter);
        formatted_implementation(format, forward<Args>(parameters)...);
    }

    ALWAYS_INLINE void formatted_implementation(StringView& format)
    {
        consume_until_format_specifier(format);
        // NOTE: The number of parameters passed to the `formatted` function doesn't match the
        //       number of format specifiers located in the format string.
        ARC_ASSERT(format.is_empty());
    }

    void consume_until_format_specifier(StringView& cursor);
    void consume_format_specifier(StringView& cursor);

private:
    ByteBuffer m_characters_buffer;
    usize m_byte_count;
};

}
