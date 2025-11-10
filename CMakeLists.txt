#
# Copyright (c) 2024 Traian Avram. All rights reserved.
# SPDX-License-Identifier: BSD-3-Clause.
#

cmake_minimum_required(VERSION 3.20)
project(arc LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall")

# Set the output directory for all targets as the same directory where
# the CMake build files are written to.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_PDB_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(ARC_SOURCE_FILES
    ast/ast_node.cpp
    ast/ast_node.h

    bytecode/disassembler.cpp
    bytecode/disassembler.h
    bytecode/instruction.cpp
    bytecode/instruction.h
    bytecode/forward.h
    bytecode/jump_address.h
    bytecode/package.cpp
    bytecode/package.h
    bytecode/register.h

    cmd/argument_parser.cpp
    cmd/argument_parser.h
    cmd/cmd_entry_point.cpp

    core/assertions.cpp
    core/assertions.h
    core/badge.h
    core/containers/array.h
    core/containers/format.cpp
    core/containers/format.h
    core/containers/optional.h
    core/containers/own_ptr.h
    core/containers/ref_ptr.h
    core/containers/span.h
    core/containers/string.cpp
    core/containers/string.h
    core/containers/string_builder.cpp
    core/containers/string_builder.h
    core/containers/string_view.cpp
    core/containers/string_view.h
    core/containers/vector.h
    core/defines.h
    core/forward.h
    core/memory/byte_buffer.cpp
    core/memory/byte_buffer.h
    core/memory/memory_operations.cpp
    core/memory/memory_operations.h
    core/numeric_limits.h
    core/types.h

    runtime/forward.h
    runtime/instruction_execute.cpp
    runtime/interpreter.cpp
    runtime/interpreter.h
    runtime/virtual_machine.cpp
    runtime/virtual_machine.h
)

add_executable(arc ${ARC_SOURCE_FILES})
target_include_directories(arc PUBLIC ${CMAKE_SOURCE_DIR})
