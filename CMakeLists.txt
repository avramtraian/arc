#
# Copyright (c) 2024 Traian Avram. All rights reserved.
# SPDX-License-Identifier: BSD-3-Clause.
#

cmake_minimum_required(VERSION 3.20)
project(arc LANGUAGES CXX C)

#==========================================================================================================================================#
#-------------------------------------------------------- BUILD CONFIGURATION TYPES -------------------------------------------------------#
#==========================================================================================================================================#

set(VALID_BUILD_TYPES Debug Release)
set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose build type")
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS ${VALID_BUILD_TYPES})

# Validate provided build type.
if (NOT CMAKE_BUILD_TYPE IN_LIST VALID_BUILD_TYPES)
    message(FATAL_ERROR "Invalid build type: ${CMAKE_BUILD_TYPE}. Valid options are: ${VALID_BUILD_TYPES}")
endif ()

message(STATUS "Configuring build type: ${CMAKE_BUILD_TYPE}")

# NOTE: Don't allow multi-configuration generators to actually have multiple configurations at the same time.
#       This simplifies the build system since there are no differences between generators.
set(CMAKE_CONFIGURATION_TYPES ${CMAKE_BUILD_TYPE})

#==========================================================================================================================================#
#------------------------------------------------------------ OUTPUT DIRECTORY ------------------------------------------------------------#
#==========================================================================================================================================#

# All generated binaries should be stored directly under the CMake binary directory.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
# NOTE: For multi-configuration generators (such as Visual Studio or XCode) there are separate output directories for each configuration.
#       Since we only have one configuration per project file there is no need for multiple directories.
string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE_UPPER)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${BUILD_TYPE_UPPER} ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${BUILD_TYPE_UPPER} ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${BUILD_TYPE_UPPER} ${CMAKE_BINARY_DIR})

#==========================================================================================================================================#
#------------------------------------------------------------ COMPILATION FLAGS -----------------------------------------------------------#
#==========================================================================================================================================#

# Compile using the C++20 standard.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -Wall -pedantic")
        add_compile_definitions("ARC_CONFIGURATION_DEBUG=1")
    elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DNDEBUG -Wall")
        add_compile_definitions("ARC_CONFIGURATION_RELEASE=1")
    endif ()
elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Od /Zi /W3 /EHsc")
        add_compile_definitions("ARC_CONFIGURATION_DEBUG=1")
    elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2 /DNDEBUG /W3 /EHsc")
        add_compile_definitions("ARC_CONFIGURATION_RELEASE=1")
    endif ()
endif ()

#==========================================================================================================================================#
#----------------------------------------------------------- TARGET DEFINITIONS -----------------------------------------------------------#
#==========================================================================================================================================#

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
    core/error.h
    core/memory/byte_buffer.cpp
    core/memory/byte_buffer.h
    core/memory/memory_operations.cpp
    core/memory/memory_operations.h
    core/numeric_limits.h
    core/types.h
    core/utf8_encoding.cpp
    core/utf8_encoding.h

    frontend/source_location.cpp
    frontend/source_location.h

    runtime/forward.h
    runtime/instruction_execute.cpp
    runtime/interpreter.cpp
    runtime/interpreter.h
    runtime/virtual_machine.cpp
    runtime/virtual_machine.h
)

add_executable(arc ${ARC_SOURCE_FILES})
target_include_directories(arc PUBLIC ${CMAKE_SOURCE_DIR})
