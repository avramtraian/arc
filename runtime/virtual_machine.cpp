/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#include <core/memory/memory_operations.h>
#include <runtime/virtual_machine.h>

namespace arc::runtime {

VirtualStack::VirtualStack(Badge<VirtualMachine>)
    : m_stack_pointer(0)
{
    // Initialize the stack buffer with 16KiB of space.
    m_buffer.allocate_new(16 * 1024);
    m_stack_pointer = m_buffer.byte_count();
}

ReadWriteBytes VirtualStack::push(usize push_byte_count)
{
    ARC_ASSERT(m_stack_pointer >= push_byte_count);
    m_stack_pointer -= push_byte_count;
    return m_buffer.bytes() + m_stack_pointer;
}

void VirtualStack::pop(usize pop_byte_count)
{
    ARC_ASSERT(m_stack_pointer + pop_byte_count <= m_buffer.byte_count());

    // NOTE: Ensure that the stack region that was popped contains no valid data.
    zero_memory(m_buffer.bytes() + m_stack_pointer, pop_byte_count);
    m_stack_pointer += pop_byte_count;
}

ReadWriteBytes VirtualStack::at_offset(usize offset, usize byte_count)
{
    if (m_stack_pointer + offset + byte_count > m_buffer.byte_count()) {
        // TODO: Throw a memory violation error instead of just crashing the runtime!
        ARC_ASSERT_NOT_REACHED;
    }

    return m_buffer.bytes() + m_stack_pointer + offset;
}

ReadonlyBytes VirtualStack::at_offset(usize offset, usize byte_count) const
{
    if (m_stack_pointer + offset + byte_count > m_buffer.byte_count()) {
        // TODO: Throw a memory violation error instead of just crashing the runtime!
        ARC_ASSERT_NOT_REACHED;
    }

    return m_buffer.bytes() + m_stack_pointer + offset;
}

VirtualCallStack::VirtualCallStack()
{}

void VirtualCallStack::push(bytecode::JumpAddress return_address, u64 parameters_byte_count)
{
    CallFrame call_frame = {};
    call_frame.return_address = return_address;
    call_frame.parameters_byte_count = parameters_byte_count;
    m_call_stack.push_back(call_frame);
}

VirtualCallStack::CallFrame VirtualCallStack::pop()
{
    if (m_call_stack.is_empty()) {
        // TODO: Provide more debug information before crashing the runtime.
        ARC_ASSERT_NOT_REACHED;
    }

    const CallFrame last_call_frame = m_call_stack.last();
    m_call_stack.pop_back();
    return last_call_frame;
}

VirtualMachine::VirtualMachine()
    : m_stack({})
{}

VirtualMachine::RegisterStorage& VirtualMachine::register_storage(bytecode::Register reg)
{
    const u8 register_index = static_cast<u8>(reg);
    ARC_ASSERT(register_index < m_registers.count());
    return m_registers[register_index];
}

const VirtualMachine::RegisterStorage& VirtualMachine::register_storage(bytecode::Register reg) const
{
    const u8 register_index = static_cast<u8>(reg);
    ARC_ASSERT(register_index < m_registers.count());
    return m_registers[register_index];
}

}
