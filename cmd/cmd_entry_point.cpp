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
    package.emit_instruction<LoadImmediate8Instruction>(Register::GPR0, 5);
    package.emit_instruction<LoadImmediate8Instruction>(Register::GPR1, 10);
    package.emit_instruction<AddInstruction>(Register::GPR2, Register::GPR0, Register::GPR1);

    const Disassembler disassembler(package);
    printf("%s", disassembler.instructions_as_string().characters());

    VirtualMachine virtual_machine;
    Interpreter interpreter(virtual_machine, package);
    interpreter.execute();

    auto dst_register = virtual_machine.register_storage(Register::GPR2);
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
