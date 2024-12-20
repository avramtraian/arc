#
# Copyright (c) 2024 Traian Avram. All rights reserved.
# SPDX-License-Identifier: BSD-3-Clause.
#

set(ARC_SOURCE_FILES
    base/argument_parser.cpp
    base/argument_parser.h
    base/assertions.cpp
    base/assertions.h
    base/containers/span.h
    base/containers/string_view.cpp
    base/containers/string_view.h
    base/containers/vector.h
    base/defines.h
    base/memory/byte_buffer.cpp
    base/memory/byte_buffer.h
    base/memory/memory_operations.cpp
    base/memory/memory_operations.h
    base/numeric_limits.h
    base/types.h

    bytecode/forward.h
    bytecode/instruction.cpp
    bytecode/instruction.h
    bytecode/jump_address.h
    bytecode/package.cpp
    bytecode/package.h
    bytecode/register.h
)

add_library(arc-core STATIC ${ARC_SOURCE_FILES})
target_include_directories(arc-core PUBLIC ${CMAKE_SOURCE_DIR})
