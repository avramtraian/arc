/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#include <bytecode/disassembler.h>
#include <bytecode/instruction.h>
#include <bytecode/package.h>
#include <cmd/argument_parser.h>
#include <runtime/interpreter.h>

#include <core/containers/string_builder.h>
#include <cstdio>

namespace arc::cmd {

void entry_point(const CommandLineArguments&)
{
    using namespace bytecode;
    using namespace runtime;

    bytecode::Package package;
    package.emit_instruction<LoadImmediate8Instruction>(Register::GPR0, 0);
    package.emit_instruction<LoadImmediate8Instruction>(Register::GPR1, 1);
    package.emit_instruction<LoadImmediate8Instruction>(Register::GPR2, 10);
    package.emit_instruction<CompareGreaterInstruction>(Register::GPR3, Register::GPR1, Register::GPR2);
    package.emit_instruction<JumpIfInstruction>(Register::GPR3, JumpAddress(8));
    package.emit_instruction<AddInstruction>(Register::GPR0, Register::GPR0, Register::GPR1);
    package.emit_instruction<IncrementInstruction>(Register::GPR1);
    package.emit_instruction<JumpInstruction>(JumpAddress(3));

    const Disassembler disassembler(package);
    printf("%s", disassembler.instructions_as_string().characters());

    VirtualMachine virtual_machine;
    Interpreter interpreter(virtual_machine, package);
    interpreter.execute();

    auto dst_register = virtual_machine.register_storage(Register::GPR0);
    printf("%s", StringBuilder::formatted("{}"sv, dst_register).characters());
}

}

int main(int argc, char** argv)
{
    arc::cmd::CommandLineArguments arguments = {};
    arguments.argument_count = argc;
    arguments.arguments = argv;
    arc::cmd::entry_point(arguments);
    return 0;
}
