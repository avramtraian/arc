/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#include <bytecode/package.h>
#include <runtime/interpreter.h>

namespace Arc::Runtime {

Interpreter::Interpreter(VirtualMachine& virtual_machine, const Bytecode::Package& package)
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

void Interpreter::jump(Bytecode::JumpAddress jump_address)
{
    // NOTE: The interpreter is already scheduled to jump. No instruction should be able
    //       to schedule two (or more) jumps so this must be a programming error.
    ARC_ASSERT(!m_jump_address.has_value());
    m_jump_address = jump_address;
}

void Interpreter::call(Bytecode::JumpAddress callee_address, u64 parameters_byte_count)
{
    // TODO: Currently, the call instruction doesn't automatically save the current state of the registers
    //       to the stack, thus extra instructions must be issued in order to preserve the necessary registers.
    //       While this is the behaviour of the x86 instruction set, the time saved by only preserving the
    //       used registers might be insignificant compared to the time required to fetch and execute instructions
    //       in our virtual machine. Automatically saving all registers to stack might provide better performance!

    // NOTE: When fetching an instruction from the package the instruction pointer is automatically
    //       incremented, thus the instruction pointer represents the next instruction after the `Call`.
    const Bytecode::JumpAddress return_address = Bytecode::JumpAddress(m_instruction_pointer);
    m_virtual_machine.call_stack().push(return_address, parameters_byte_count);
    jump(callee_address);
}

void Interpreter::return_from_call()
{
    // Get the last call frame from the virtual machine's call stack.
    const VirtualCallStack::CallFrame last_call_frame = m_virtual_machine.call_stack().pop();

    // Pop the call parameters from the stack.
    m_virtual_machine.stack().pop(last_call_frame.parameters_byte_count);

    // Jump back to the call return address.
    jump(last_call_frame.return_address);
}

void Interpreter::fetch_and_execute()
{
    const Bytecode::Instruction& instruction = m_package.fetch_instruction(m_instruction_pointer);
    ++m_instruction_pointer;
    instruction.execute(*this);

    if (m_jump_address.has_value()) {
        m_instruction_pointer = m_jump_address.value().address();
        m_jump_address.clear();
    }
}

}
