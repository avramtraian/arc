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

void LoadImmediate8Instruction::execute(Interpreter& interpreter) const
{
    auto& dst = interpreter.vm().register_storage(m_dst_register);
    dst.value = static_cast<u64>(m_immediate_value);
}

}
