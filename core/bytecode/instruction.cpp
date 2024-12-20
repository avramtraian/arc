/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#include <core/base/assertions.h>
#include <core/bytecode/instruction.h>

namespace arc {

const char* instruction_code_to_string(InstructionCode instruction_code)
{
    switch (instruction_code) {
#define ARC_SWITCH_CASE(x)  \
    case InstructionCode::x: \
        return #x;
        ARC_ENUMERATE_INSTRUCTION_CODES(ARC_SWITCH_CASE)
#undef ARC_SWITCH_CASE
    }

    ARC_ASSERT_NOT_REACHED;
    return nullptr;
}

InstructionCode instruction_code_from_string(StringView instruction_string)
{
#define ARC_IF_STATEMENT(x)          \
    if (instruction_string == #x##sv) \
        return InstructionCode::x;
    ARC_ENUMERATE_INSTRUCTION_CODES(ARC_IF_STATEMENT)
#undef ARC_IF_STATEMENT

    ARC_ASSERT_NOT_REACHED;
    return InstructionCode::Unknown;
}

}
