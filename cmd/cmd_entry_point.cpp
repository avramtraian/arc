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

    // int n = 10, a = 0, b = 1;
    /* [ 0] */ package.emit_instruction<PushImmediate64Instruction>(15); // offset 0 (n)
    /* [ 1] */ package.emit_instruction<PushImmediate64Instruction>(0); // offset 8 (a)
    /* [ 2] */ package.emit_instruction<PushImmediate64Instruction>(1); // offset 16 (b)

    // int i = 1;
    // while (i <= n) {
    /* [ 3] */ package.emit_instruction<PushImmediate64Instruction>(1); // offset 24 (i)
    /* [ 4] */ package.emit_instruction<LoadFromStackInstruction>(Register::GPR0, 0); // load n
    /* [ 5] */ package.emit_instruction<LoadFromStackInstruction>(Register::GPR1, 24); // load i
    /* [ 6] */ package.emit_instruction<CompareGreaterInstruction>(Register::GPR0, Register::GPR1, Register::GPR0);
    /* [ 7] */ package.emit_instruction<JumpIfInstruction>(Register::GPR0, JumpAddress(20));

    // int temp = a;
    /* [ 8] */ package.emit_instruction<LoadFromStackInstruction>(Register::GPR0, 8); // load a
    /* [ 9] */ package.emit_instruction<PushRegisterInstruction>(Register::GPR0); // offset 32 (temp)

    // a = b;
    /* [10] */ package.emit_instruction<LoadFromStackInstruction>(Register::GPR0, 16); // load b
    /* [11] */ package.emit_instruction<StoreToStackInstruction>(8, Register::GPR0); // store in a

    // b = temp + b;
    /* [12] */ package.emit_instruction<LoadFromStackInstruction>(Register::GPR1, 32); // load temp
    /* [13] */ package.emit_instruction<AddInstruction>(Register::GPR0, Register::GPR1, Register::GPR0);
    /* [14] */ package.emit_instruction<StoreToStackInstruction>(16, Register::GPR0); // store in b

    // ++i; }
    /* [15] */ package.emit_instruction<PopRegisterInstruction>(); // pop (temp)
    /* [16] */ package.emit_instruction<LoadFromStackInstruction>(Register::GPR0, 24); // load i
    /* [17] */ package.emit_instruction<IncrementInstruction>(Register::GPR0);
    /* [18] */ package.emit_instruction<StoreToStackInstruction>(24, Register::GPR0); // store in i
    /* [19] */ package.emit_instruction<JumpInstruction>(JumpAddress(4));

    // Load the value of b in GPR0 in order to print it to the console.
    package.emit_instruction<LoadFromStackInstruction>(Register::GPR0, 16); // load b
    // Pop the stack.
    package.emit_instruction<PopRegisterInstruction>();
    package.emit_instruction<PopRegisterInstruction>();
    package.emit_instruction<PopRegisterInstruction>();
    package.emit_instruction<PopRegisterInstruction>();

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
