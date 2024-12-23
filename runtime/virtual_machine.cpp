/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#include <runtime/virtual_machine.h>

namespace arc::runtime {

VirtualMachine::VirtualMachine()
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
