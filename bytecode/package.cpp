/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#include <bytecode/package.h>

namespace arc::bytecode {

Package::Package()
{}

bool Package::instruction_pointer_is_valid(usize instruction_pointer) const
{
    const bool instruction_pointer_is_in_range = instruction_pointer < m_instructions.count();
    return instruction_pointer_is_in_range;
}

const Instruction& Package::fetch_instruction(usize instruction_pointer) const
{
    ARC_ASSERT(instruction_pointer_is_valid(instruction_pointer));
    return m_instructions.at(instruction_pointer).dereference();
}

}
