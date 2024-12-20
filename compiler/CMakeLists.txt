#
# Copyright (c) 2024 Traian Avram. All rights reserved.
# SPDX-License-Identifier: BSD-3-Clause.
#

set(ARC_COMPILER_SOURCE_FILES
    main.cpp
)

add_executable(arc-compiler ${ARC_COMPILER_SOURCE_FILES})
target_include_directories(arc-compiler PUBLIC ${CMAKE_SOURCE_DIR})
set_target_properties(arc-compiler PROPERTIES OUTPUT_NAME arcc)

add_dependencies(arc-compiler arc-core)
target_include_directories(arc-compiler PUBLIC arc-core)
