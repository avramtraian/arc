/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#include <core/memory/memory_operations.h>
#include <runtime/virtual_machine.h>

namespace arc::runtime {

VirtualStack::VirtualStack(Badge<VirtualMachine>)
    : m_byte_offset(0)
{
    // Initialize the stack buffer with 16KiB of space.
    m_buffer.allocate_new(16 * 1024);
}

ReadWriteBytes VirtualStack::push(usize push_byte_count)
{
    // TODO: Ensure that the stack pointer will not overflow.

    m_buffer.ensure_byte_count(m_byte_offset + push_byte_count);
    const ReadWriteBytes bytes = m_buffer.bytes() + m_byte_offset;
    m_byte_offset += push_byte_count;
    return bytes;
}

void VirtualStack::pop(usize pop_byte_count)
{
    if (pop_byte_count > m_byte_offset) {
        // TODO: Throw a memory violation error instead of just crashing the runtime!
        ARC_ASSERT_NOT_REACHED;
    }

    m_byte_offset -= pop_byte_count;
    // NOTE: Ensure that the stack region that was popped contains no valid data.
    zero_memory(m_buffer.bytes() + m_byte_offset, pop_byte_count);
}

ReadWriteBytes VirtualStack::at_offset(usize offset, usize byte_count)
{
    if (offset + byte_count > m_byte_offset) {
        // TODO: Throw a memory violation error instead of just crashing the runtime!
        ARC_ASSERT_NOT_REACHED;
    }

    return m_buffer.bytes() + offset;
}

ReadonlyBytes VirtualStack::at_offset(usize offset, usize byte_count) const
{
    if (offset + byte_count > m_byte_offset) {
        // TODO: Throw a memory violation error instead of just crashing the runtime!
        ARC_ASSERT_NOT_REACHED;
    }

    return m_buffer.bytes() + offset;
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
