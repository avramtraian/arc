/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#include <core/bytecode/instruction.h>
#include <runtime/interpreter.h>

namespace arc {

Interpreter::Interpreter(const Package& package)
    : m_package(package)
    , m_instruction_byte_offset(0)
{
    m_registers.set_count_defaulted(16);
}

void Interpreter::execute()
{
    while (m_instruction_byte_offset < m_package.byte_count) {
        const ReadonlyBytes encoded_instruction = m_package.byte_buffer.bytes() + m_instruction_byte_offset;
        const InstructionCode instruction_code = *reinterpret_cast<const InstructionCode*>(encoded_instruction);
        switch (instruction_code) {
            case InstructionCode::Add: {
                const auto& instruction = decode_instruction<AddInstruction>();
                auto& dst = register_storage(instruction.dst_register);
                const auto& lhs = register_storage(instruction.lhs_register);
                const auto& rhs = register_storage(instruction.rhs_register);
                dst.value = lhs.value + rhs.value;
                break;
            }

            case InstructionCode::CompareGreater: {
                const auto& instruction = decode_instruction<CompareGreaterInstruction>();
                auto& dst = register_storage(instruction.dst_register);
                const auto& lhs = register_storage(instruction.lhs_register);
                const auto& rhs = register_storage(instruction.rhs_register);
                dst.value = lhs.value > rhs.value;
                 break;
            }

            case InstructionCode::Increment: {
                const auto& instruction = decode_instruction<IncrementInstruction>();
                auto& dst = register_storage(instruction.dst_register);
                ++dst.value;
                break;
            }

            case InstructionCode::JumpAbsolute: {
                const auto& instruction = decode_instruction<JumpAbsoluteInstruction>();
                m_instruction_byte_offset = instruction.jump_address.value;
                break;
            }

            case InstructionCode::JumpAbsoluteIf: {
                const auto& instruction = decode_instruction<JumpAbsoluteIfInstruction>();
                const auto& condition = register_storage(instruction.condition_register);
                if (condition.value) {
                    m_instruction_byte_offset = instruction.jump_address.value;
                }
                break;
            }

            case InstructionCode::LoadImmediate8: {
                const auto& instruction = decode_instruction<LoadImmediate8Instruction>();
                auto& dst = register_storage(instruction.dst_register);
                dst.value = static_cast<u64>(instruction.immediate_value);
                break;
            }
        }
    }
}

Interpreter::RegisterStorage& Interpreter::register_storage(Register in_register)
{
    ARC_ASSERT(in_register < m_registers.count());
    return m_registers[in_register];
}

const Interpreter::RegisterStorage& Interpreter::register_storage(Register in_register) const
{
    ARC_ASSERT(in_register < m_registers.count());
    return m_registers[in_register];
}

}
