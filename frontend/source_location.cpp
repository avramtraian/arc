/*
 * Copyright (c) 2024-2025 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#include <core/containers/string_builder.h>
#include <core/utf8_encoding.h>
#include <frontend/source_location.h>

namespace Arc::Frontend {

ErrorOr<SourceLocation> SourceLocation::create_from_byte_offset(StringView file_source_code, usize byte_offset)
{
    if (byte_offset >= file_source_code.byte_count()) {
        // TODO: Report to the caller that the provided byte offset exceeds the size of the source code buffer.
        return ARC_INTERNAL_ERROR();
    }

    SourceLocation source_location = {};
    source_location.byte_offset = byte_offset;

    auto file_source_code_reader = Utf8StreamReader(file_source_code.byte_span());
    while (file_source_code_reader.is_valid() && file_source_code_reader.stream_byte_offset() < byte_offset) {
        const Utf8DecodeResult decode_result = file_source_code_reader.decode_result();
        source_location.column_index++;
        if (decode_result.codepoint == '\n') {
            source_location.line_index++;
            source_location.column_index = 0;

            // NOTE: Handle the CRLF new-line sequence.
            if (file_source_code_reader.peek_next().codepoint_or_invalid() == '\r')
                file_source_code_reader.advance();
        }
        file_source_code_reader.advance();
    }

    if (file_source_code_reader.has_encoding_error()) {
        // TODO: Report to the caller that the provided source code is not correctly encoded (UTF-8)!
        //       Think about whether this error should be reported as an internal error or compilation error.
        return ARC_INTERNAL_ERROR();
    }

    ARC_ASSERT(file_source_code_reader.stream_byte_offset() == byte_offset);
    return source_location;
}

String SourceLocation::to_user_readable_string(StringView filepath) const
{
    StringBuilder builder;
    builder.append("{}:{}:{}"sv, filepath, line_index, column_index);
    return builder.release_string();
}

String SourceLocation::to_debug_string(StringView filepath) const
{
    StringBuilder builder;
    builder.append("{}:{}:{}({})"sv, filepath, line_index, column_index, byte_offset);
    return builder.release_string();
}

ErrorOr<SourceRegion> SourceRegion::create_from_source(String filepath, StringView file_source_code, SourceLocation start_location,
                                                       SourceLocation end_location)
{
    ARC_ASSERT(end_location.byte_offset >= start_location.byte_offset);
    const usize source_region_byte_count = end_location.byte_offset - start_location.byte_offset;
    const StringView source_region =
        StringView::from_utf8(file_source_code.characters() + start_location.byte_offset, source_region_byte_count);
    return SourceRegion(move(filepath), source_region, start_location, end_location);
}

ErrorOr<SourceRegion> SourceRegion::create_from_byte_offsets(String filepath, StringView file_source_code, usize start_byte_offset,
                                                             usize end_byte_offset)
{
    TRY_ASSIGN(const SourceLocation start_location, SourceLocation::create_from_byte_offset(file_source_code, start_byte_offset));
    TRY_ASSIGN(const SourceLocation end_location, SourceLocation::create_from_byte_offset(file_source_code, end_byte_offset));
    TRY_RETURN(create_from_source(move(filepath), file_source_code, start_location, end_location));
}

SourceRegion::SourceRegion(String filepath, StringView source_region, SourceLocation start_location, SourceLocation end_location)
    : m_filepath(move(filepath))
    , m_source_region(source_region)
    , m_start_location(start_location)
    , m_end_location(end_location)
{}

}
