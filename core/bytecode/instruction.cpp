/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#include <core/base/assertions.h>
#include <core/bytecode/instruction.h>

namespace arc {

StringView instruction_code_to_string(InstructionCode instruction_code)
{
    switch (instruction_code) {
#define ARC_SWITCH_CASE(x, unused) \
    case InstructionCode::x:       \
        return #x##sv;
        ARC_ENUMERATE_INSTRUCTION_CODES(ARC_SWITCH_CASE)
#undef ARC_SWITCH_CASE
    }

    ARC_ASSERT_NOT_REACHED;
    return {};
}

InstructionCode instruction_code_from_string(StringView instruction_string)
{
#define ARC_IF_STATEMENT(x, unused)   \
    if (instruction_string == #x##sv) \
        return InstructionCode::x;
    ARC_ENUMERATE_INSTRUCTION_CODES(ARC_IF_STATEMENT)
#undef ARC_IF_STATEMENT

    ARC_ASSERT_NOT_REACHED;
    return InstructionCode::Unknown;
}

}
