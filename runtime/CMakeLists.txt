#
# Copyright (c) 2024 Traian Avram. All rights reserved.
# SPDX-License-Identifier: BSD-3-Clause.
#

set(ARC_RUNTIME_SOURCE_FILES
    interpreter.cpp
    interpreter.h
    main.cpp
)

add_executable(arc-runtime ${ARC_RUNTIME_SOURCE_FILES})
target_include_directories(arc-runtime PUBLIC ${CMAKE_SOURCE_DIR})
set_target_properties(arc-runtime PROPERTIES OUTPUT_NAME arcr)

add_dependencies(arc-runtime arc-core)
target_link_libraries(arc-runtime PUBLIC arc-core)
