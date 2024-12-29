/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#include <bytecode/instruction.h>
#include <core/containers/string_builder.h>

namespace arc::bytecode {

String AddInstruction::to_string() const
{
    return StringBuilder::formatted("AddInstruction dst:{}, lhs:{}, rhs:{}"sv, m_dst_register, m_lhs_register, m_rhs_register);
}

String CompareGreaterInstruction::to_string() const
{
    return StringBuilder::formatted("CompareGreater dst:{}, lhs:{}, rhs:{}"sv, m_dst_register, m_lhs_register, m_rhs_register);
}

String DecrementInstruction::to_string() const
{
    return StringBuilder::formatted("Decrement dst:{}"sv, m_dst_register);
}

String IncrementInstruction::to_string() const
{
    return StringBuilder::formatted("Increment dst:{}"sv, m_dst_register);
}

String JumpInstruction::to_string() const
{
    return StringBuilder::formatted("Jump address:{}"sv, m_jump_address);
}

String LoadFromStackInstruction::to_string() const
{
    return StringBuilder::formatted("LoadFromStack dst:{}, src:{}"sv, m_dst_register, m_src_stack_offset);
}

String JumpIfInstruction::to_string() const
{
    return StringBuilder::formatted("JumpIf condition:{}, address:{}"sv, m_condition_register, m_jump_address);
}

String LoadImmediate8Instruction::to_string() const
{
    return StringBuilder::formatted("LoadImmediate8 dst:{}, value:{}"sv, m_dst_register, m_immediate_value);
}

String PopRegisterInstruction::to_string() const
{
    return StringBuilder::formatted("PopRegister"sv);
}

String PushImmediate8Instruction::to_string() const
{
    return StringBuilder::formatted("PushImmediate8 value:{}"sv, m_immediate_value);
}

String PushImmediate16Instruction::to_string() const
{
    return StringBuilder::formatted("PushImmediate16value:{}"sv, m_immediate_value);
}

String PushImmediate32Instruction::to_string() const
{
    return StringBuilder::formatted("PushImmediate32 value:{}"sv, m_immediate_value);
}

String PushImmediate64Instruction::to_string() const
{
    return StringBuilder::formatted("PushImmediate64 value:{}"sv, m_immediate_value);
}

String PushRegisterInstruction::to_string() const
{
    return StringBuilder::formatted("PushRegister src:{}"sv, m_src_register);
}

String StoreToStackInstruction::to_string() const
{
    return StringBuilder::formatted("StoreToStack dst:{}, src:{}"sv, m_dst_stack_offset, m_src_register);
}

String Store8ToStackInstruction::to_string() const
{
    return StringBuilder::formatted("Store8ToStack dst:{}, src:{}"sv, m_dst_stack_offset, m_src_register);
}

String Store16ToStackInstruction::to_string() const
{
    return StringBuilder::formatted("Store16ToStack dst:{}, src:{}"sv, m_dst_stack_offset, m_src_register);
}

String Store32ToStackInstruction::to_string() const
{
    return StringBuilder::formatted("Store32ToStack dst:{}, src:{}"sv, m_dst_stack_offset, m_src_register);
}

String SubInstruction::to_string() const
{
    return StringBuilder::formatted("Sub dst:{}, lhs:{}, rhs:{}"sv, m_dst_register, m_lhs_register, m_rhs_register);
}

}
