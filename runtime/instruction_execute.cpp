/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#include <bytecode/instruction.h>
#include <runtime/interpreter.h>
#include <runtime/virtual_machine.h>

namespace Arc::Bytecode {

using namespace Arc::Runtime;

void AddInstruction::execute(Interpreter& interpreter) const
{
    auto& dst = interpreter.vm().register_storage(m_dst_register);
    const auto& lhs = interpreter.vm().register_storage(m_lhs_register);
    const auto& rhs = interpreter.vm().register_storage(m_rhs_register);
    dst.value = lhs.value + rhs.value;
}

void CallInstruction::execute(Runtime::Interpreter& interpreter) const
{
    interpreter.call(m_callee_address, m_parameters_byte_count);
}

void CompareGreaterInstruction::execute(Runtime::Interpreter& interpreter) const
{
    auto& dst = interpreter.vm().register_storage(m_dst_register);
    const auto& lhs = interpreter.vm().register_storage(m_lhs_register);
    const auto& rhs = interpreter.vm().register_storage(m_rhs_register);
    dst.value = lhs.value > rhs.value;
}

void DecrementInstruction::execute(Runtime::Interpreter& interpreter) const
{
    auto& dst = interpreter.vm().register_storage(m_dst_register);
    --dst.value;
}

void IncrementInstruction::execute(Runtime::Interpreter& interpreter) const
{
    auto& dst = interpreter.vm().register_storage(m_dst_register);
    ++dst.value;
}

void JumpInstruction::execute(Runtime::Interpreter& interpreter) const
{
    interpreter.jump(m_jump_address);
}

void JumpIfInstruction::execute(Runtime::Interpreter& interpreter) const
{
    const auto& condition = interpreter.vm().register_storage(m_condition_register);
    if (condition.value)
        interpreter.jump(m_jump_address);
}

void LoadFromStackInstruction::execute(Runtime::Interpreter& interpreter) const
{
    VirtualMachine& vm = interpreter.vm();
    auto& dst_register = vm.register_storage(m_dst_register);
    const auto& src_register = vm.stack().at_offset<VirtualMachine::RegisterStorage>(m_src_stack_offset);
    dst_register.value = src_register.value;
}

void Load8FromStackInstruction::execute(Runtime::Interpreter& interpreter) const
{
    VirtualMachine& vm = interpreter.vm();
    auto& dst_register = vm.register_storage(m_dst_register);
    const auto& src_value = vm.stack().at_offset<u8>(m_src_stack_offset);
    dst_register.value = static_cast<u64>(src_value);
}

void Load16FromStackInstruction::execute(Runtime::Interpreter& interpreter) const
{
    VirtualMachine& vm = interpreter.vm();
    auto& dst_register = vm.register_storage(m_dst_register);
    const auto& src_value = vm.stack().at_offset<u16>(m_src_stack_offset);
    dst_register.value = static_cast<u64>(src_value);
}

void Load32FromStackInstruction::execute(Runtime::Interpreter& interpreter) const
{
    VirtualMachine& vm = interpreter.vm();
    auto& dst_register = vm.register_storage(m_dst_register);
    const auto& src_value = vm.stack().at_offset<u32>(m_src_stack_offset);
    dst_register.value = static_cast<u64>(src_value);
}

void LoadImmediate8Instruction::execute(Interpreter& interpreter) const
{
    auto& dst = interpreter.vm().register_storage(m_dst_register);
    dst.value = static_cast<u64>(m_immediate_value);
}

void PopInstruction::execute(Runtime::Interpreter& interpreter) const
{
    interpreter.vm().stack().pop(m_pop_byte_count);
}

void PopRegisterInstruction::execute(Interpreter& interpreter) const
{
    VirtualMachine& vm = interpreter.vm();
    vm.stack().pop<VirtualMachine::RegisterStorage>();
}

void PushInstruction::execute(Runtime::Interpreter& interpreter) const
{
    interpreter.vm().stack().push(m_push_byte_count);
}

void PushImmediate8Instruction::execute(Runtime::Interpreter& interpreter) const
{
    VirtualMachine& vm = interpreter.vm();
    u8& dst = vm.stack().push<u8>();
    dst = m_immediate_value;
}

void PushImmediate16Instruction::execute(Runtime::Interpreter& interpreter) const
{
    VirtualMachine& vm = interpreter.vm();
    u16& dst = vm.stack().push<u16>();
    dst = m_immediate_value;
}

void PushImmediate32Instruction::execute(Runtime::Interpreter& interpreter) const
{
    VirtualMachine& vm = interpreter.vm();
    u32& dst = vm.stack().push<u32>();
    dst = m_immediate_value;
}

void PushImmediate64Instruction::execute(Runtime::Interpreter& interpreter) const
{
    VirtualMachine& vm = interpreter.vm();
    u64& dst = vm.stack().push<u64>();
    dst = m_immediate_value;
}

void PushRegisterInstruction::execute(Interpreter& interpreter) const
{
    VirtualMachine& vm = interpreter.vm();
    auto& dst_register = vm.stack().push<VirtualMachine::RegisterStorage>();
    const auto& src_register = vm.register_storage(m_src_register);
    dst_register.value = src_register.value;
}

void ReturnInstruction::execute(Runtime::Interpreter& interpreter) const
{
    interpreter.return_from_call();
}

void StoreToStackInstruction::execute(Interpreter& interpreter) const
{
    VirtualMachine& vm = interpreter.vm();
    auto& dst_register = vm.stack().at_offset<VirtualMachine::RegisterStorage>(m_dst_stack_offset);
    const auto& src_register = vm.register_storage(m_src_register);
    dst_register.value = src_register.value;
}

void Store8ToStackInstruction::execute(Interpreter& interpreter) const
{
    VirtualMachine& vm = interpreter.vm();
    u8& dst = vm.stack().at_offset<u8>(m_dst_stack_offset);
    const auto& src_register = vm.register_storage(m_src_register);
    dst = static_cast<u8>(src_register.value);
}

void Store16ToStackInstruction::execute(Interpreter& interpreter) const
{
    VirtualMachine& vm = interpreter.vm();
    u16& dst = vm.stack().at_offset<u16>(m_dst_stack_offset);
    const auto& src_register = vm.register_storage(m_src_register);
    dst = static_cast<u16>(src_register.value);
}

void Store32ToStackInstruction::execute(Interpreter& interpreter) const
{
    VirtualMachine& vm = interpreter.vm();
    u32& dst = vm.stack().at_offset<u32>(m_dst_stack_offset);
    const auto& src_register = vm.register_storage(m_src_register);
    dst = static_cast<u32>(src_register.value);
}

void SubInstruction::execute(Runtime::Interpreter& interpreter) const
{
    auto& dst = interpreter.vm().register_storage(m_dst_register);
    const auto& lhs = interpreter.vm().register_storage(m_lhs_register);
    const auto& rhs = interpreter.vm().register_storage(m_rhs_register);
    dst.value = lhs.value - rhs.value;
}

}
