/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#pragma once

#include <bytecode/jump_address.h>
#include <bytecode/register.h>
#include <core/badge.h>
#include <core/containers/array.h>
#include <core/containers/format.h>
#include <core/containers/vector.h>
#include <runtime/forward.h>

namespace arc::runtime {

class VirtualStack {
    ARC_MAKE_NONCOPYABLE(VirtualStack);
    ARC_MAKE_NONMOVABLE(VirtualStack);

public:
    VirtualStack(Badge<VirtualMachine>);
    ~VirtualStack() = default;

    ReadWriteBytes push(usize push_byte_count);
    void pop(usize pop_byte_count);

    NODISCARD ReadWriteBytes at_offset(usize offset, usize byte_count);
    NODISCARD ReadonlyBytes at_offset(usize offset, usize byte_count) const;

public:
    template<typename T>
    requires (is_trivially_destructible<T>)
    ALWAYS_INLINE T& push()
    {
        const ReadWriteBytes bytes = push(sizeof(T));
        return *reinterpret_cast<T*>(bytes);
    }

    template<typename T>
    requires (is_trivially_destructible<T>)
    ALWAYS_INLINE void pop()
    {
        pop(sizeof(T));
    }

    template<typename T>
    NODISCARD ALWAYS_INLINE T& at_offset(usize offset)
    {
        const ReadWriteBytes bytes = at_offset(offset, sizeof(T));
        return *reinterpret_cast<T*>(bytes);
    }

    template<typename T>
    NODISCARD ALWAYS_INLINE const T& at_offset(usize offset) const
    {
        const ReadonlyBytes bytes = at_offset(offset, sizeof(T));
        return *reinterpret_cast<const T*>(bytes);
    }

private:
    ByteBuffer m_buffer;
    u64 m_stack_pointer;
};

class VirtualCallStack {
    ARC_MAKE_NONCOPYABLE(VirtualCallStack);
    ARC_MAKE_NONMOVABLE(VirtualCallStack);

public:
    struct CallFrame {
        bytecode::JumpAddress return_address { 0 };
        u64 parameters_byte_count { 0 };
    };

public:
    VirtualCallStack();

    void push(bytecode::JumpAddress return_address, u64 parameters_byte_count);
    NODISCARD CallFrame pop();

private:
    Vector<CallFrame> m_call_stack;
};

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

    NODISCARD ALWAYS_INLINE VirtualStack& stack() { return m_stack; }
    NODISCARD ALWAYS_INLINE const VirtualStack& stack() const { return m_stack; }

    NODISCARD ALWAYS_INLINE VirtualCallStack& call_stack() { return m_call_stack; }
    NODISCARD ALWAYS_INLINE const VirtualCallStack& call_stack() const { return m_call_stack; }

private:
    Array<RegisterStorage, static_cast<u8>(bytecode::Register::Count)> m_registers;
    VirtualStack m_stack;
    VirtualCallStack m_call_stack;
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
