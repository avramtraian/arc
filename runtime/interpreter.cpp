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
#define DISPATCH_EXECUTE_INSTRUCTION(pascal_name, snake_name)                     \
    case InstructionCode::pascal_name: {                                          \
        const auto& instruction = decode_instruction<pascal_name##Instruction>(); \
        execute_##snake_name##_instruction(instruction);                          \
        break;                                                                    \
    }
            ARC_ENUMERATE_INSTRUCTION_CODES(DISPATCH_EXECUTE_INSTRUCTION)
#undef DISPATCH_EXECUTE_INSTRUCTION

            default: {
                // An invalid instruction has been encountered!
                ARC_ASSERT_NOT_REACHED;
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

void Interpreter::execute_add_instruction(const AddInstruction& instruction)
{
    auto& dst = register_storage(instruction.dst_register);
    const auto& lhs = register_storage(instruction.lhs_register);
    const auto& rhs = register_storage(instruction.rhs_register);
    dst.value = lhs.value + rhs.value;
}

void Interpreter::execute_break_instruction(const BreakInstruction&)
{
    // TODO: Break the interpreter execution loop instead of just crashing the program.
    ARC_ASSERT_NOT_REACHED;
}

void Interpreter::execute_compare_greater_instruction(const CompareGreaterInstruction& instruction)
{
    auto& dst = register_storage(instruction.dst_register);
    const auto& lhs = register_storage(instruction.lhs_register);
    const auto& rhs = register_storage(instruction.rhs_register);
    dst.value = lhs.value > rhs.value;
}

void Interpreter::execute_increment_instruction(const IncrementInstruction& instruction)
{
    auto& dst = register_storage(instruction.dst_register);
    ++dst.value;
}

void Interpreter::execute_jump_absolute_instruction(const JumpAbsoluteInstruction& instruction)
{
    m_instruction_byte_offset = instruction.jump_address.value;
}

void Interpreter::execute_jump_absolute_if_instruction(const JumpAbsoluteIfInstruction& instruction)
{
    const auto& condition = register_storage(instruction.condition_register);
    if (condition.value)
        m_instruction_byte_offset = instruction.jump_address.value;
}

void Interpreter::execute_load_immediate_8_instruction(const LoadImmediate8Instruction& instruction)
{
    auto& dst = register_storage(instruction.dst_register);
    dst.value = instruction.immediate_value;
}

void Interpreter::execute_unknown_instruction(const UnknownInstruction&)
{
    // NOTE: The provided bytecode is invalid or corrupt. Report the error to the user
    //       instead of just crashing the runtime program without any context.
    ARC_ASSERT_NOT_REACHED;
}

}
