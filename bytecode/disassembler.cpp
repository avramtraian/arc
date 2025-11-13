/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#include <bytecode/disassembler.h>
#include <bytecode/package.h>
#include <core/containers/string_builder.h>

namespace Arc::Bytecode {

Disassembler::Disassembler(const Package& package)
    : m_package(package)
{}

String Disassembler::instructions_as_string() const
{
    StringBuilder builder;
    usize instruction_pointer = 0;
    while (m_package.instruction_pointer_is_valid(instruction_pointer)) {
        const auto& instruction = m_package.fetch_instruction(instruction_pointer);
        builder.append("[{}] {}\n"sv, instruction_pointer, instruction.to_string());
        ++instruction_pointer;
    }

    return builder.release_string();
}

}
