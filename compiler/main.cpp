/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#include <core/base/argument_parser.h>

namespace arc {

static void compiler_main(const CommandLineArguments&)
{}

}

int main(int argc, char** argv)
{
    arc::CommandLineArguments arguments = {};
    arguments.arguments = argv;
    arguments.argument_count = static_cast<arc::u32>(argc);
    arc::compiler_main(arguments);
    return 0;
}
