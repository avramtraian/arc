/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#include <core/base/assertions.h>

#include <cstdio>

namespace arc {

void arc_assertion_failed(AssertionKind kind, const char* expression, const char* file, const char* function,
                          u32 line_number)
{
    if (kind == AssertionKind::Assert) {
        printf("ASSERTION FAILED: %s\n", expression);
    }
    else if (kind == AssertionKind::AssertDebug) {
        printf("ASSERTION DEBUG FAILED: %s\n", expression);
    }
    else if (kind == AssertionKind::AssertNotReached) {
        printf("ASSERTION NOT REACHED!\n");
    }

    printf("  - File:     %s\n", file);
    printf("  - Function: %s\n", function);
    printf("  - Line:     %u\n", line_number);
}

}
