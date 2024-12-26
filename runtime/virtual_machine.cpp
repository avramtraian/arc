/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#include <core/memory/memory_operations.h>
#include <runtime/virtual_machine.h>

namespace arc::runtime {

VirtualMachine::VirtualMachine()
    : m_stack_byte_count(0)
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

VirtualMachine::RegisterStorage& VirtualMachine::stack_as_register_storage(usize stack_byte_offset)
{
    // NOTE: Check against buffer overflows.
    ARC_ASSERT(stack_byte_offset + sizeof(RegisterStorage) <= m_stack_byte_count);
    return *reinterpret_cast<RegisterStorage*>(m_stack_buffer.elements() + stack_byte_offset);
}

const VirtualMachine::RegisterStorage& VirtualMachine::stack_as_register_storage(usize stack_byte_offset) const
{
    // NOTE: Check against buffer overflows.
    ARC_ASSERT(stack_byte_offset + sizeof(RegisterStorage) <= m_stack_byte_count);
    return *reinterpret_cast<const RegisterStorage*>(m_stack_buffer.elements() + stack_byte_offset);
}

usize VirtualMachine::stack_byte_count() const
{
    return m_stack_byte_count;
}

WriteonlyBytes VirtualMachine::stack_push(usize push_byte_count)
{
    m_stack_buffer.set_count(m_stack_byte_count + push_byte_count, 0);
    const WriteonlyBytes bytes = m_stack_buffer.elements() + m_stack_byte_count;
    m_stack_byte_count += push_byte_count;
    return bytes;
}

void VirtualMachine::stack_pop(usize pop_byte_count)
{
    // NOTE: Check against buffer overflows.
    ARC_ASSERT(pop_byte_count <= m_stack_byte_count);
    m_stack_byte_count -= pop_byte_count;
    zero_memory(m_stack_buffer.elements() + m_stack_byte_count, pop_byte_count);
}

VirtualMachine::RegisterStorage& VirtualMachine::stack_push_register()
{
    const WriteonlyBytes bytes = stack_push(sizeof(RegisterStorage));
    return *reinterpret_cast<RegisterStorage*>(bytes);
}

}
