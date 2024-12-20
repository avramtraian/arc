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
    const Package& m_package;
    u64 m_instruction_byte_offset;

    Vector<RegisterStorage> m_registers;
};

}
