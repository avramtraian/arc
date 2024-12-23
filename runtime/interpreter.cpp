/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#include <bytecode/package.h>
#include <runtime/interpreter.h>

namespace arc::runtime {

Interpreter::Interpreter(VirtualMachine& virtual_machine, const bytecode::Package& package)
    : m_virtual_machine(virtual_machine)
    , m_package(package)
    , m_instruction_pointer(0)
{}

void Interpreter::execute()
{
    // Always reset the instruction pointer.
    m_instruction_pointer = 0;

    while (m_package.instruction_pointer_is_valid(m_instruction_pointer)) {
        const bytecode::Instruction& instruction = m_package.fetch_instruction(m_instruction_pointer);
        ++m_instruction_pointer;
        instruction.execute(*this);
    }
}

}
