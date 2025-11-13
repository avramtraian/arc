/*
 * Copyright (c) 2024-2025 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#pragma once

#include <core/containers/string.h>
#include <core/error.h>

namespace Arc::Frontend {

struct SourceLocation {
public:
    NODISCARD static ErrorOr<SourceLocation> create_from_byte_offset(StringView file_source_code, usize byte_offset);

public:
    NODISCARD String to_user_readable_string(StringView filepath) const;
    NODISCARD String to_debug_string(StringView filepath) const;

public:
    u32 line_index { 0 };
    u32 column_index { 0 };
    usize byte_offset { 0 };
};

class SourceRegion {
public:
    NODISCARD static ErrorOr<SourceRegion> create_from_source(String filepath, StringView file_source_code, SourceLocation start_location,
                                                              SourceLocation end_location);

    NODISCARD static ErrorOr<SourceRegion> create_from_byte_offsets(String filepath, StringView file_source_code, usize start_byte_offset,
                                                                    usize end_byte_offset);

public:
    SourceRegion() = default;
    SourceRegion(String filepath, StringView source_region, SourceLocation start_location, SourceLocation end_location);

private:
    String m_filepath;
    StringView m_source_region;
    SourceLocation m_start_location;
    SourceLocation m_end_location;
};

}
