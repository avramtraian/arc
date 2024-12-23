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

String LoadImmediate8Instruction::to_string() const
{
    return StringBuilder::formatted("LoadImmediate8 dst:{}, value:{}"sv, m_dst_register, m_immediate_value);
}

}
