/*
 * Copyright (c) 2024-2025 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#pragma once

#include <core/containers/span.h>
#include <core/error.h>

namespace Arc {

using UnicodeCodepoint = u32;
static constexpr UnicodeCodepoint INVALID_UNICODE_CODEPOINT = static_cast<UnicodeCodepoint>(-1);

struct Utf8DecodeResult {
public:
    NODISCARD ALWAYS_INLINE bool is_valid() const { return (codepoint != INVALID_UNICODE_CODEPOINT && codepoint_byte_width > 0); }
    NODISCARD ALWAYS_INLINE UnicodeCodepoint codepoint_or_invalid() const { return is_valid() ? codepoint : INVALID_UNICODE_CODEPOINT; }
    NODISCARD ALWAYS_INLINE usize codepoint_byte_width_or_zero() const { return is_valid() ? codepoint_byte_width : 0; }

public:
    UnicodeCodepoint codepoint { INVALID_UNICODE_CODEPOINT };
    usize codepoint_byte_width { 0 };
};

NODISCARD Utf8DecodeResult utf8_decode_byte_sequence(const u8* encoded_byte_buffer, usize byte_count);
NODISCARD Utf8DecodeResult utf8_decode_byte_sequence(ReadonlyByteSpan encoded_byte_buffer);

ErrorOr<Utf8DecodeResult> try_utf8_decode_byte_sequence(const u8* encoded_byte_buffer, usize byte_count);
ErrorOr<Utf8DecodeResult> try_utf8_decode_byte_sequence(ReadonlyByteSpan encoded_byte_buffer);

class Utf8StreamReader {
public:
    explicit Utf8StreamReader(ReadonlyByteSpan encoded_byte_buffer);

public:
    NODISCARD ALWAYS_INLINE usize stream_byte_offset() const { return m_stream_byte_offset; }
    NODISCARD ALWAYS_INLINE bool has_encoding_error() const { return m_current_offset_has_encoding_error; }

    NODISCARD ALWAYS_INLINE bool is_valid() const
    {
        ARC_ASSERT_DEBUG(m_stream_byte_offset < m_encoded_byte_buffer.byte_count());
        return m_current_offset_decode_result.is_valid();
    }

    NODISCARD Utf8DecodeResult decode_result() const
    {
        ARC_ASSERT(is_valid());
        return m_current_offset_decode_result;
    }

public:
    void advance();

    NODISCARD Utf8DecodeResult peek_next();

private:
    ReadonlyByteSpan m_encoded_byte_buffer;
    usize m_stream_byte_offset;
    Utf8DecodeResult m_current_offset_decode_result;
    bool m_current_offset_has_encoding_error;
    usize m_current_codepoint_index;
};

}
