/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#pragma once

#include <core/base/types.h>

namespace arc {

struct CommandLineArguments {
    char** arguments;
    u32 argument_count;
};

}
