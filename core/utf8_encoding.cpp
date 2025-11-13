/*
 * Copyright (c) 2024-2025 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#include <core/utf8_encoding.h>

namespace Arc {

Utf8DecodeResult utf8_decode_byte_sequence(const u8* encoded_byte_buffer, usize byte_count)
{
    // NOTE: The decode result that will be returned by this function if an encoding error is encountered.
    Utf8DecodeResult invalid_decode_result;
    invalid_decode_result.codepoint = INVALID_UNICODE_CODEPOINT;
    invalid_decode_result.codepoint_byte_width = 0;

    if (byte_count == 0)
        return invalid_decode_result;

    if ((encoded_byte_buffer[0] & 0x80) == 0x00) {
        Utf8DecodeResult decode_result;
        decode_result.codepoint = encoded_byte_buffer[0];
        decode_result.codepoint_byte_width = 1;
        return decode_result;
    }

    if ((encoded_byte_buffer[0] & 0xE0) == 0xC0) {
        if (byte_count < 2)
            return invalid_decode_result;

        Utf8DecodeResult decode_result;
        decode_result.codepoint += (encoded_byte_buffer[0] & 0x1F) << 6;
        decode_result.codepoint += (encoded_byte_buffer[1] & 0x3F) << 0;

        decode_result.codepoint_byte_width = 2;
        return decode_result;
    }

    if ((encoded_byte_buffer[0] & 0xF0) == 0xE0) {
        if (byte_count < 3)
            return invalid_decode_result;

        Utf8DecodeResult decode_result;
        decode_result.codepoint += (encoded_byte_buffer[0] & 0x1F) << 12;
        decode_result.codepoint += (encoded_byte_buffer[1] & 0x3F) << 6;
        decode_result.codepoint += (encoded_byte_buffer[2] & 0x3F) << 0;

        decode_result.codepoint_byte_width = 3;
        return decode_result;
    }

    if ((encoded_byte_buffer[0] & 0xF8) == 0xF0) {
        if (byte_count < 4)
            return invalid_decode_result;

        Utf8DecodeResult decode_result;
        decode_result.codepoint += (encoded_byte_buffer[0] & 0x1F) << 18;
        decode_result.codepoint += (encoded_byte_buffer[1] & 0x3F) << 12;
        decode_result.codepoint += (encoded_byte_buffer[2] & 0x3F) << 6;
        decode_result.codepoint += (encoded_byte_buffer[3] & 0x3F) << 0;

        decode_result.codepoint_byte_width = 4;
        return decode_result;
    }

    return invalid_decode_result;
}

Utf8DecodeResult utf8_decode_byte_sequence(ReadonlyByteSpan encoded_byte_buffer)
{
    // Forward the implementation to the pointer and byte count override.
    const Utf8DecodeResult result = utf8_decode_byte_sequence(encoded_byte_buffer.elements(), encoded_byte_buffer.byte_count());
    return result;
}

ErrorOr<Utf8DecodeResult> try_utf8_decode_byte_sequence(const u8* encoded_byte_buffer, usize byte_count)
{
    // Forward the implementation and wrap the result.
    const Utf8DecodeResult result = utf8_decode_byte_sequence(encoded_byte_buffer, byte_count);
    if (!result.is_valid())
        return ARC_INTERNAL_ERROR();
    return result;
}

ErrorOr<Utf8DecodeResult> try_utf8_decode_byte_sequence(ReadonlyByteSpan encoded_byte_buffer)
{
    // Forward the implementation and wrap the result.
    const Utf8DecodeResult result = utf8_decode_byte_sequence(encoded_byte_buffer);
    if (!result.is_valid())
        return ARC_INTERNAL_ERROR();
    return result;
}

Utf8StreamReader::Utf8StreamReader(ReadonlyByteSpan encoded_byte_buffer)
    : m_encoded_byte_buffer(encoded_byte_buffer)
    , m_stream_byte_offset(0)
    , m_current_offset_has_encoding_error(false)
    , m_current_codepoint_index(0)
{
    if (m_stream_byte_offset < m_encoded_byte_buffer.byte_count()) {
        m_current_offset_decode_result = utf8_decode_byte_sequence(m_encoded_byte_buffer.elements() + m_stream_byte_offset,
                                                                   m_encoded_byte_buffer.byte_count() - m_stream_byte_offset);
        if (!m_current_offset_decode_result.is_valid())
            m_current_offset_has_encoding_error = true;
    }
}

void Utf8StreamReader::advance()
{
    if (is_valid()) {
        m_stream_byte_offset += m_current_offset_decode_result.codepoint_byte_width;
        m_current_offset_decode_result = {};

        if (m_stream_byte_offset < m_encoded_byte_buffer.byte_count()) {
            m_current_offset_decode_result = utf8_decode_byte_sequence(m_encoded_byte_buffer.elements() + m_stream_byte_offset,
                                                                       m_encoded_byte_buffer.byte_count() - m_stream_byte_offset);
            if (!m_current_offset_decode_result.is_valid())
                m_current_offset_has_encoding_error = true;
            ++m_current_codepoint_index;
        }
    }
}

Utf8DecodeResult Utf8StreamReader::peek_next()
{
    Utf8DecodeResult next_decode_result = {};
    if (is_valid()) {
        const usize peek_byte_offset = m_stream_byte_offset + m_current_offset_decode_result.codepoint_byte_width;
        if (peek_byte_offset < m_encoded_byte_buffer.byte_count()) {
            next_decode_result = utf8_decode_byte_sequence(m_encoded_byte_buffer.elements() + peek_byte_offset,
                                                           m_encoded_byte_buffer.byte_count() - peek_byte_offset);
        }
    }
    return next_decode_result;
}

}
