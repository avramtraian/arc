/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#pragma once

#include <core/bytecode/package.h>

namespace arc {

class Interpreter {
public:
    struct RegisterStorage {
        u64 value;
    };

public:
    explicit Interpreter(const Package& package);

    void execute();

    NODISCARD RegisterStorage& register_storage(Register in_register);
    NODISCARD const RegisterStorage& register_storage(Register in_register) const;

private:
    template<typename InstructionType>
    NODISCARD ALWAYS_INLINE const InstructionType& decode_instruction()
    {
        // TODO(Traian): Actually validate that the instruction stream contains the required instruction
        //               type. Decoding the wrong instruction type will cause bugs very hard to track down.

        const ReadonlyBytes encoded_instruction = m_package.byte_buffer.bytes() + m_instruction_byte_offset;
        const InstructionType& instruction = *reinterpret_cast<const InstructionType*>(encoded_instruction);
        m_instruction_byte_offset += sizeof(InstructionType);
        return instruction;
    }

private:
    void execute_add_instruction(const AddInstruction& instruction);
    void execute_break_instruction(const BreakInstruction& instruction);
    void execute_compare_greater_instruction(const CompareGreaterInstruction& instruction);
    void execute_increment_instruction(const IncrementInstruction& instruction);
    void execute_jump_absolute_instruction(const JumpAbsoluteInstruction& instruction);
    void execute_jump_absolute_if_instruction(const JumpAbsoluteIfInstruction& instruction);
    void execute_load_immediate_8_instruction(const LoadImmediate8Instruction& instruction);
    void execute_unknown_instruction(const UnknownInstruction& instruction);

private:
    const Package& m_package;
    u64 m_instruction_byte_offset;

    Vector<RegisterStorage> m_registers;
};

}
