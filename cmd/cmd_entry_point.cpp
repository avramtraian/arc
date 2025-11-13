/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#include <ast/ast_node.h>
#include <bytecode/disassembler.h>
#include <bytecode/instruction.h>
#include <bytecode/package.h>
#include <cmd/argument_parser.h>
#include <runtime/interpreter.h>

#include <core/containers/string_builder.h>
#include <cstdio>

namespace Arc::Cmd {

using namespace Bytecode;
using namespace Runtime;

MAYBE_UNUSED static Register compile_fibonacci_linear(Package& package, u64& out_entry_point)
{
    // int n = 10, a = 0, b = 1;
    // int i = 1;
    /* [ 0] */ package.emit_instruction<PushImmediate64Instruction>(15); // offset 24 (n)
    /* [ 1] */ package.emit_instruction<PushImmediate64Instruction>(0); // offset 16 (a)
    /* [ 2] */ package.emit_instruction<PushImmediate64Instruction>(1); // offset 8 (b)
    /* [ 3] */ package.emit_instruction<PushImmediate64Instruction>(1); // offset 0 (i)

    // while (i <= n) {
    /* [ 4] */ package.emit_instruction<LoadFromStackInstruction>(Register::GPR0, 24); // load n
    /* [ 5] */ package.emit_instruction<LoadFromStackInstruction>(Register::GPR1, 0); // load i
    /* [ 6] */ package.emit_instruction<CompareGreaterInstruction>(Register::GPR0, Register::GPR1, Register::GPR0);
    /* [ 7] */ package.emit_instruction<JumpIfInstruction>(Register::GPR0, JumpAddress(20));

    // int temp = a;
    /* [ 8] */ package.emit_instruction<LoadFromStackInstruction>(Register::GPR0, 16); // load a
    /* [ 9] */ package.emit_instruction<PushRegisterInstruction>(Register::GPR0); // offset 0 (temp)

    // n = offset 32
    // a = offset 24
    // b = offset 16
    // i = offset 8

    // a = b;
    /* [10] */ package.emit_instruction<LoadFromStackInstruction>(Register::GPR0, 16); // load b
    /* [11] */ package.emit_instruction<StoreToStackInstruction>(24, Register::GPR0); // store in a

    // b = temp + b;
    /* [12] */ package.emit_instruction<LoadFromStackInstruction>(Register::GPR1, 0); // load temp
    /* [13] */ package.emit_instruction<AddInstruction>(Register::GPR0, Register::GPR1, Register::GPR0);
    /* [14] */ package.emit_instruction<StoreToStackInstruction>(16, Register::GPR0); // store in b

    // ++i; }
    /* [15] */ package.emit_instruction<PopRegisterInstruction>(); // pop (temp)
    /* [16] */ package.emit_instruction<LoadFromStackInstruction>(Register::GPR0, 0); // load i
    /* [17] */ package.emit_instruction<IncrementInstruction>(Register::GPR0);
    /* [18] */ package.emit_instruction<StoreToStackInstruction>(0, Register::GPR0); // store in i
    /* [19] */ package.emit_instruction<JumpInstruction>(JumpAddress(4));

    // Load the value of b in GPR0 in order to print it to the console.
    package.emit_instruction<LoadFromStackInstruction>(Register::GPR0, 8); // load b
    // Pop the stack.
    package.emit_instruction<PopRegisterInstruction>();
    package.emit_instruction<PopRegisterInstruction>();
    package.emit_instruction<PopRegisterInstruction>();
    package.emit_instruction<PopRegisterInstruction>();

    out_entry_point = 0;
    return Register::GPR0;
}

MAYBE_UNUSED static Register compile_fibonacci_recursive(Package& package, u64& out_entry_point)
{
    // u64 fib(u64 k) {
    // if (k == 0 || k == 1)
    //   return k;
    // return fib(k-1) + fib(k-2);
    // }
    // u64 result = fib(n);

    // result (offset 8)
    // k (offset 0)

    // u64 k;
    package.emit_instruction<LoadFromStackInstruction>(Register::GPR0, 0); // load k

    // if (k > 1) {
    package.emit_instruction<LoadImmediate8Instruction>(Register::GPR1, 1);
    package.emit_instruction<CompareGreaterInstruction>(Register::GPR1, Register::GPR0, Register::GPR1);
    package.emit_instruction<JumpIfInstruction>(Register::GPR1, JumpAddress(6));
    // return k; }
    package.emit_instruction<StoreToStackInstruction>(8, Register::GPR0); // store into result
    package.emit_instruction<ReturnInstruction>();

    // u64 t1 = fib(--k);
    package.emit_instruction<DecrementInstruction>(Register::GPR0);

    // Save the GPR0 register as it will be modified during the recursive call.
    package.emit_instruction<PushRegisterInstruction>(Register::GPR0);

    package.emit_instruction<PushInstruction>(8);
    package.emit_instruction<PushRegisterInstruction>(Register::GPR0);
    package.emit_instruction<CallInstruction>(JumpAddress(0), 8);
    package.emit_instruction<LoadFromStackInstruction>(Register::GPR2, 0);
    package.emit_instruction<PopInstruction>(8);

    // Restore the GPR0 register after the recursive call.
    package.emit_instruction<LoadFromStackInstruction>(Register::GPR0, 0);
    package.emit_instruction<PopRegisterInstruction>();

    // u64 t2 = fib(--k);
    package.emit_instruction<DecrementInstruction>(Register::GPR0);

    // Save the GPR0 and GPR2 registers as they will be modified during the recursive call.
    package.emit_instruction<PushRegisterInstruction>(Register::GPR0);
    package.emit_instruction<PushRegisterInstruction>(Register::GPR2);

    package.emit_instruction<PushInstruction>(8);
    package.emit_instruction<PushRegisterInstruction>(Register::GPR0);
    package.emit_instruction<CallInstruction>(JumpAddress(0), 8);
    package.emit_instruction<LoadFromStackInstruction>(Register::GPR3, 0);
    package.emit_instruction<PopInstruction>(8);

    // Restore the GPR0 and GPR2 registers after the recursive call.
    package.emit_instruction<LoadFromStackInstruction>(Register::GPR2, 0);
    package.emit_instruction<LoadFromStackInstruction>(Register::GPR0, 8);
    package.emit_instruction<PopRegisterInstruction>();
    package.emit_instruction<PopRegisterInstruction>();

    // return t1 + t2;
    package.emit_instruction<AddInstruction>(Register::GPR0, Register::GPR2, Register::GPR3);
    package.emit_instruction<StoreToStackInstruction>(8, Register::GPR0); // store into result
    package.emit_instruction<ReturnInstruction>();

    // u64 result = fib(n)
    package.emit_instruction<PushInstruction>(8); // push return value space
    package.emit_instruction<PushImmediate64Instruction>(11); // push n
    package.emit_instruction<CallInstruction>(JumpAddress(0), 8);
    package.emit_instruction<LoadFromStackInstruction>(Register::GPR0, 0); // load return value
    package.emit_instruction<PopInstruction>(8);

    out_entry_point = 30;
    return Register::GPR0;
}

MAYBE_UNUSED static void generate_fibonacci_ast()
{
    /*
        int fib(int n) {
            int prev_fib = 1;
            int curr_fib = 1;
            int k = 1;
            while (k < n) {
                int new_fib = prev_fib + curr_fib;
                prev_fib = curr_fib;
                curr_fib = new_fib;
            }
            return curr_fib;
        }
        int result = fib(20);
    */

    // clang-format off
    Vector<AST::FunctionDeclaration::Parameter> function_parameters;
    function_parameters.push_back({ "int"sv, "n"sv });

    auto function_body = AST::create_node<AST::ExecutionScope>();

    {
        // int prev_fib = 1;
        auto literal_signed_int_1 = AST::create_node<AST::LiteralExpression>(AST::LiteralType::SignedInteger);
        literal_signed_int_1->set_signed_integer(1);
        auto prev_fib_assignment_expression = AST::create_node<AST::AssignmentExpression>(
            AST::create_node<AST::VariableDeclaration>("int"sv, "prev_fib"sv),
            move(literal_signed_int_1)
        );
        function_body->add_child(move(prev_fib_assignment_expression));
    }

    {
        // int curr_fib = 1;
        auto literal_signed_int_1 = AST::create_node<AST::LiteralExpression>(AST::LiteralType::SignedInteger);
        literal_signed_int_1->set_signed_integer(1);
        auto prev_fib_assignment_expression = AST::create_node<AST::AssignmentExpression>(
            AST::create_node<AST::VariableDeclaration>("int"sv, "curr_fib"sv),
            move(literal_signed_int_1)
        );
        function_body->add_child(move(prev_fib_assignment_expression));
    }

    auto while_body = AST::create_node<AST::ExecutionScope>();

    {
        // int new_fib = prev_fib + curr_fib;
        auto assignment_expression = AST::create_node<AST::AssignmentExpression>(
            AST::create_node<AST::VariableDeclaration>("int"sv, "new_fib"sv),
            AST::create_node<AST::BinaryExpression>(
                AST::BinaryOperation::Add,
                AST::create_node<AST::IdentifierExpression>("prev_fib"sv),
                AST::create_node<AST::IdentifierExpression>("curr_fib"sv)
            )
        );
        while_body->add_child(move(assignment_expression));
    }

    {
        // prev_fib = curr_fib;
        auto assignment_expression = AST::create_node<AST::AssignmentExpression>(
            AST::create_node<AST::IdentifierExpression>("prev_fib"sv),
            AST::create_node<AST::IdentifierExpression>("curr_fib"sv)
        );
        while_body->add_child(move(assignment_expression));
    }

    {
        // curr_fib = new_fib;
        auto assignment_expression = AST::create_node<AST::AssignmentExpression>(
            AST::create_node<AST::IdentifierExpression>("curr_fib"sv),
            AST::create_node<AST::IdentifierExpression>("new_fib"sv)
        );
        while_body->add_child(move(assignment_expression));
    }

    // k < n
    auto while_condition_expression = AST::create_node<AST::BinaryExpression>(
        AST::BinaryOperation::CompareLess,
        AST::create_node<AST::IdentifierExpression>("k"sv),
        AST::create_node<AST::IdentifierExpression>("n"sv)
    );

    function_body->add_child(AST::create_node<AST::WhileStructure>(move(while_condition_expression), move(while_body)));
    function_body->add_child(AST::create_node<AST::ReturnStatement>(AST::create_node<AST::IdentifierExpression>("curr_fib"sv)));

    auto function_declaration = AST::create_node<AST::FunctionDeclaration>("int"sv, "fib"sv, function_parameters, move(function_body));

    auto result_call = AST::create_node<AST::CallExpression>(AST::create_node<AST::IdentifierExpression>("fib"sv));
    auto index_literal = AST::create_node<AST::LiteralExpression>(AST::LiteralType::UnsignedInteger);
    result_call->add_parameter(move(index_literal));

    auto result_assignment = AST::create_node<AST::AssignmentExpression>(
        AST::create_node<AST::VariableDeclaration>("int"sv, "result"sv),
        move(result_call)
    );

    auto program = AST::create_node<AST::ExecutionScope>();
    program->add_child(move(function_declaration));
    program->add_child(move(result_assignment));

    // clang-format on

    StringBuilder builder;
    program->dump_as_string(builder, 0, 4);
    printf("\n%s\n", builder.release_string().characters());
}

void entry_point(const CommandLineArguments&)
{
    Package package;
    u64 entry_point = 0;
    // const Register result_register = compile_fibonacci_linear(package, entry_point);
    const Register result_register = compile_fibonacci_recursive(package, entry_point);

    const Disassembler disassembler(package);
    printf("%s", disassembler.instructions_as_string().characters());

    VirtualMachine virtual_machine;
    Interpreter interpreter(virtual_machine, package);
    interpreter.set_entry_point(entry_point);
    interpreter.execute();

    auto dst_register = virtual_machine.register_storage(result_register);
    printf("%s", StringBuilder::formatted("{}"sv, dst_register).characters());

    generate_fibonacci_ast();
}

}

int main(int argc, char** argv)
{
    Arc::Cmd::CommandLineArguments arguments = {};
    arguments.argument_count = argc;
    arguments.arguments = argv;
    Arc::Cmd::entry_point(arguments);
    return 0;
}
