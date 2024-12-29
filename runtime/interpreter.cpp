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
{
    // Always reset the instruction pointer.
    m_instruction_pointer = 0;
}

void Interpreter::set_entry_point(u64 entry_point_instruction_offset)
{
    // TODO: Ensure that the provided entry point instruction offset is actually valid.
    m_instruction_pointer = entry_point_instruction_offset;
}

void Interpreter::execute()
{
    while (m_package.instruction_pointer_is_valid(m_instruction_pointer)) {
        fetch_and_execute();
    }
}

void Interpreter::jump(bytecode::JumpAddress jump_address)
{
    // NOTE: The interpreter is already scheduled to jump. No instruction should be able
    //       to schedule two (or more) jumps so this must be a programming error.
    ARC_ASSERT(!m_jump_address.has_value());
    m_jump_address = jump_address;
}

void Interpreter::fetch_and_execute()
{
    const bytecode::Instruction& instruction = m_package.fetch_instruction(m_instruction_pointer);
    ++m_instruction_pointer;
    instruction.execute(*this);

    if (m_jump_address.has_value()) {
        m_instruction_pointer = m_jump_address.value().address();
        m_jump_address.clear();
    }
}

}
