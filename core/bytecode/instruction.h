/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#pragma once

#include <core/base/containers/string_view.h>
#include <core/base/types.h>
#include <core/bytecode/jump_address.h>
#include <core/bytecode/register.h>

namespace arc {

// clang-format off
#define ARC_ENUMERATE_INSTRUCTION_CODES(x) \
    x(Unknown, unknown)                    \
    x(Add, add)                            \
    x(CompareGreater, compare_greater)     \
    x(Increment, increment)                \
    x(JumpAbsolute, jump_absolute)         \
    x(JumpAbsoluteIf, jump_absolute_if)    \
    x(LoadImmediate8, load_immediate_8)    \
    x(Break, break)
// clang-format on

enum class InstructionCode : u16 {
#define ARC_ENUM_DECLARATION(x, unused) x,
    ARC_ENUMERATE_INSTRUCTION_CODES(ARC_ENUM_DECLARATION)
#undef ARC_ENUM_DECLARATION
};

StringView instruction_code_to_string(InstructionCode instruction_code);
InstructionCode instruction_code_from_string(StringView instruction_string);

struct AddInstruction {
public:
    const InstructionCode code { InstructionCode::Add };
    Register dst_register;
    Register lhs_register;
    Register rhs_register;

public:
    ALWAYS_INLINE AddInstruction(Register in_dst_register, Register in_lhs_register, Register in_rhs_register)
        : dst_register(in_dst_register)
        , lhs_register(in_lhs_register)
        , rhs_register(in_rhs_register)
    {}
};

struct CompareGreaterInstruction {
public:
    const InstructionCode code { InstructionCode::CompareGreater };
    Register dst_register;
    Register lhs_register;
    Register rhs_register;

public:
    ALWAYS_INLINE
    CompareGreaterInstruction(Register in_dst_register, Register in_lhs_register, Register in_rhs_register)
        : dst_register(in_dst_register)
        , lhs_register(in_lhs_register)
        , rhs_register(in_rhs_register)
    {}
};

struct IncrementInstruction {
public:
    const InstructionCode code { InstructionCode::Increment };
    Register dst_register;

public:
    explicit ALWAYS_INLINE IncrementInstruction(Register in_dst_register)
        : dst_register(in_dst_register)
    {}
};

struct JumpAbsoluteInstruction {
public:
    const InstructionCode code { InstructionCode::JumpAbsolute };
    JumpAddress jump_address;

public:
    explicit ALWAYS_INLINE JumpAbsoluteInstruction(JumpAddress in_jump_address)
        : jump_address(in_jump_address)
    {}
};

struct JumpAbsoluteIfInstruction {
public:
    const InstructionCode code { InstructionCode::JumpAbsoluteIf };
    Register condition_register;
    JumpAddress jump_address;

public:
    ALWAYS_INLINE JumpAbsoluteIfInstruction(Register in_condition_register, JumpAddress in_jump_address)
        : condition_register(in_condition_register)
        , jump_address(in_jump_address)
    {}
};

struct LoadImmediate8Instruction {
public:
    const InstructionCode code { InstructionCode::LoadImmediate8 };
    Register dst_register;
    u8 immediate_value;

public:
    ALWAYS_INLINE LoadImmediate8Instruction(Register in_dst_register, u8 in_immediate_value)
        : dst_register(in_dst_register)
        , immediate_value(in_immediate_value)
    {}
};

struct BreakInstruction {
public:
    const InstructionCode code { InstructionCode::Break };
};

struct UnknownInstruction {
public:
    const InstructionCode code { InstructionCode::Unknown };
};

}
