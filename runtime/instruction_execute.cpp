/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#include <bytecode/instruction.h>
#include <runtime/interpreter.h>
#include <runtime/virtual_machine.h>

namespace arc::bytecode {

using namespace arc::runtime;

void AddInstruction::execute(Interpreter& interpreter) const
{
    auto& dst = interpreter.vm().register_storage(m_dst_register);
    const auto& lhs = interpreter.vm().register_storage(m_lhs_register);
    const auto& rhs = interpreter.vm().register_storage(m_rhs_register);
    dst.value = lhs.value + rhs.value;
}

void CompareGreaterInstruction::execute(runtime::Interpreter& interpreter) const
{
    auto& dst = interpreter.vm().register_storage(m_dst_register);
    const auto& lhs = interpreter.vm().register_storage(m_lhs_register);
    const auto& rhs = interpreter.vm().register_storage(m_rhs_register);
    dst.value = lhs.value > rhs.value;
}

void IncrementInstruction::execute(runtime::Interpreter& interpreter) const
{
    auto& dst = interpreter.vm().register_storage(m_dst_register);
    ++dst.value;
}

void JumpInstruction::execute(runtime::Interpreter& interpreter) const
{
    interpreter.jump(m_jump_address);
}

void JumpIfInstruction::execute(runtime::Interpreter& interpreter) const
{
    const auto& condition = interpreter.vm().register_storage(m_condition_register);
    if (condition.value)
        interpreter.jump(m_jump_address);
}

void LoadFromStackInstruction::execute(runtime::Interpreter& interpreter) const
{
    VirtualMachine& vm = interpreter.vm();
    auto& dst_register = vm.register_storage(m_dst_register);
    ARC_ASSERT(m_src_stack_offset + sizeof(VirtualMachine::RegisterStorage) <= vm.stack_byte_count());
    const auto& src_register = vm.stack_as_register_storage(m_src_stack_offset);
    dst_register.value = src_register.value;
}

void LoadImmediate8Instruction::execute(Interpreter& interpreter) const
{
    auto& dst = interpreter.vm().register_storage(m_dst_register);
    dst.value = static_cast<u64>(m_immediate_value);
}

void PopRegisterInstruction::execute(Interpreter& interpreter) const
{
    VirtualMachine& vm = interpreter.vm();
    vm.stack_pop(sizeof(VirtualMachine::RegisterStorage));
}

void PushImmediate8Instruction::execute(runtime::Interpreter& interpreter) const
{
    VirtualMachine& vm = interpreter.vm();
    const WriteonlyBytes bytes = vm.stack_push(sizeof(u8));
    *reinterpret_cast<u8*>(bytes) = m_immediate_value;
}

void PushImmediate16Instruction::execute(runtime::Interpreter& interpreter) const
{
    VirtualMachine& vm = interpreter.vm();
    const WriteonlyBytes bytes = vm.stack_push(sizeof(u16));
    *reinterpret_cast<u16*>(bytes) = m_immediate_value;
}

void PushImmediate32Instruction::execute(runtime::Interpreter& interpreter) const
{
    VirtualMachine& vm = interpreter.vm();
    const WriteonlyBytes bytes = vm.stack_push(sizeof(u32));
    *reinterpret_cast<u32*>(bytes) = m_immediate_value;
}

void PushImmediate64Instruction::execute(runtime::Interpreter& interpreter) const
{
    VirtualMachine& vm = interpreter.vm();
    const WriteonlyBytes bytes = vm.stack_push(sizeof(u64));
    *reinterpret_cast<u64*>(bytes) = m_immediate_value;
}

void PushRegisterInstruction::execute(Interpreter& interpreter) const
{
    VirtualMachine& vm = interpreter.vm();
    auto& dst_register = vm.stack_push_register();
    const auto& src_register = vm.register_storage(m_src_register);
    dst_register.value = src_register.value;
}

void StoreToStackInstruction::execute(Interpreter& interpreter) const
{
    VirtualMachine& vm = interpreter.vm();
    auto& dst_register = vm.stack_as_register_storage(m_dst_stack_offset);
    const auto& src_register = vm.register_storage(m_src_register);
    dst_register.value = src_register.value;
}

}
