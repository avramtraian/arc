/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#include <core/base/argument_parser.h>
#include <core/bytecode/instruction.h>
#include <core/bytecode/package.h>
#include <runtime/interpreter.h>

#include <cstdio>

namespace arc {

template<typename InstructionType, typename... Args>
void emit_instruction_in_package(Package& package, Args&&... args)
{
    package.byte_buffer.ensure_byte_count(package.byte_count + sizeof(InstructionType));
    new (package.byte_buffer.bytes() + package.byte_count) InstructionType(forward<Args>(args)...);
    package.byte_count += sizeof(InstructionType);
}

static void runtime_main(const CommandLineArguments& arguments)
{
    // [ 0] LoadImmediate8 r0, i10
    // [ 4] LoadImmediate8 r1, 0
    // [ 8] LoadImmediate8 r2, 1
    // [12] CompareGreater r3, r2, r0
    // [18] JumpAbsoluteIf r3, @8
    // [34] Add r1, r1, r2
    // [40] Increment r2
    // [44] JumpAbsolute @3
    // [60] ;; Somehow print the content of the r1 register.

    Package package;
    emit_instruction_in_package<LoadImmediate8Instruction>(package, Register(0), 100);
    emit_instruction_in_package<LoadImmediate8Instruction>(package, Register(1), 0);
    emit_instruction_in_package<LoadImmediate8Instruction>(package, Register(2), 1);
    emit_instruction_in_package<CompareGreaterInstruction>(package, Register(3), Register(2), Register(0));
    emit_instruction_in_package<JumpAbsoluteIfInstruction>(package, Register(3), JumpAddress(60));
    emit_instruction_in_package<AddInstruction>(package, Register(1), Register(1), Register(2));
    emit_instruction_in_package<IncrementInstruction>(package, Register(2));
    emit_instruction_in_package<JumpAbsoluteInstruction>(package, JumpAddress(12));

    Interpreter interpreter(package);
    interpreter.execute();

    printf(
        "Gauss sum for '%zu' is '%zu'!\n", interpreter.register_storage(Register(0)).value,
        interpreter.register_storage(Register(1)).value
    );
}

}

int main(int argc, char** argv)
{
    arc::CommandLineArguments arguments = {};
    arguments.arguments = argv;
    arguments.argument_count = static_cast<arc::u32>(argc);
    arc::runtime_main(arguments);
    return 0;
}
