/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#pragma once

#include <bytecode/register.h>
#include <core/containers/array.h>
#include <core/containers/format.h>
#include <core/containers/vector.h>

namespace arc::runtime {

class VirtualMachine {
    ARC_MAKE_NONCOPYABLE(VirtualMachine);
    ARC_MAKE_NONMOVABLE(VirtualMachine);

public:
    struct RegisterStorage {
        u64 value { 0 };
    };

public:
    VirtualMachine();

    NODISCARD RegisterStorage& register_storage(bytecode::Register);
    NODISCARD const RegisterStorage& register_storage(bytecode::Register) const;

    NODISCARD RegisterStorage& stack_as_register_storage(usize stack_byte_offset);
    NODISCARD const RegisterStorage& stack_as_register_storage(usize stack_byte_offset) const;
    NODISCARD usize stack_byte_count() const;

    WriteonlyBytes stack_push(usize push_byte_count);
    void stack_pop(usize pop_byte_count);

    RegisterStorage& stack_push_register();

private:
    Array<RegisterStorage, static_cast<u8>(bytecode::Register::Count)> m_registers;

    Vector<u8> m_stack_buffer;
    usize m_stack_byte_count;
};

}

namespace arc {
template<>
class Formatter<runtime::VirtualMachine::RegisterStorage> {
public:
    ALWAYS_INLINE static void format(FormatStream& stream, const runtime::VirtualMachine::RegisterStorage& value)
    {
        // NOTE: Currently, registers can only hold 64-bit unsigned integer values. However, this behaviour
        // will most likely change in the future, as the bytecode virtual machine will implement more instructions.
        stream.push_unsigned_integer(value.value);
        stream.push_string("_u64"sv);
    }
};
}
